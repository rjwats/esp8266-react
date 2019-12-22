const { resolve } = require('path');
const { readdir, unlink } = require('fs').promises;
const fs = require('fs');
var path = require('path');
var zlib = require('zlib');
var gzip = zlib.createGzip();

async function* getFiles(dir) {
  const entries = await readdir(dir, { withFileTypes: true });
  for (const entry of entries) {
    const entryPath = resolve(dir, entry.name);
    if (entry.isDirectory()) {
      yield* getFiles(entryPath);
    } else {
      yield entryPath;
    }
  }
}

function coherseToBuffer(input) {
  return Buffer.isBuffer(input) ? input : Buffer.from(input);
}

class ProgmemGenerator {

  constructor(options = {}) {
    const { outputPath, bytesPerLine = 15 } = options;
    this.options = {
      outputPath,
      bytesPerLine
    };
  }

  apply(compiler) {
    compiler.hooks.emit.tapAsync(
      { name: 'ProgmemGenerator' },
      (compilation, callback) => {
        const buildPath = compilation.options.output.path;
        const wwwPath = path.resolve(compilation.options.context, this.options.outputPath);

        const createProgmemFile = (relativeFilePath, buffer) => {
          const copyToPath = path.resolve(wwwPath, relativeFilePath);
          const copyToDir = path.resolve(copyToPath, "../");

          if (!fs.existsSync(copyToDir)) {
            fs.mkdirSync(copyToDir);
          }
          const writeStream = fs.createWriteStream(copyToPath, { flags: "wx" });
          zlib.gzip(buffer, (error, result) => {
            try {
              var written = 0;
              result.forEach((b, index) => {
                if (index && !(index % this.options.bytesPerLine)) {
                  writeStream.write("\n");
                }
                writeStream.write("0x" + ("00" + b.toString(16).toUpperCase()).substr(-2) + ",");
                written++;
              });
            } finally {
              writeStream.end();
            }
          });
        };



        // remove existing files
        (async () => {
          // delete previous resources
          for await (const filePath of getFiles(wwwPath)) {
            await unlink(filePath);
          }

          // copy static data
          for await (const filePath of getFiles(buildPath)) {
            const readStream = fs.readFileSync(filePath);
            const relativeFilePath = path.relative(buildPath, filePath);
            createProgmemFile(relativeFilePath, readStream);
          }

          // copy assets
          const { assets } = compilation;
          Object.keys(assets).forEach(relativeFilePath => {
            createProgmemFile(relativeFilePath, coherseToBuffer(assets[relativeFilePath].source()));
          });

          // trigger the next process
          callback();
        })();
      }
    );
  }
}

module.exports = ProgmemGenerator;
