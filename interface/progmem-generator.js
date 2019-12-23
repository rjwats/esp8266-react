const { resolve, relative, sep } = require('path');
const { readdirSync, readFileSync, unlinkSync, existsSync, mkdirSync, createWriteStream } = require('fs');
var zlib = require('zlib');
var mime = require('mime-types');

function getFilesSync(dir, files = []) {
  readdirSync(dir, { withFileTypes: true }).forEach(entry => {
    const entryPath = resolve(dir, entry.name);
    if (entry.isDirectory()) {
      getFilesSync(entryPath, files);
    } else {
      files.push(entryPath);
    }
  })
  return files;
}

function coherseToBuffer(input) {
  return Buffer.isBuffer(input) ? input : Buffer.from(input);
}

function createParentDirectories(filePath) {
  const parentDirectory = resolve(filePath, "../");
  if (!existsSync(parentDirectory)) {
    mkdirSync(parentDirectory, { recursive: true });
  }
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
        const wwwPath = resolve(compilation.options.context, this.options.outputPath);
        const fileInfo = [];

        const processFile = (relativeFilePath, buffer) => {
          const copyToPath = resolve(wwwPath, relativeFilePath);
          createParentDirectories(copyToPath);

          const variable = "ESP_REACT_DATA_" + fileInfo.length;
          const mimeType = mime.lookup(relativeFilePath);
          const writeStream = createWriteStream(copyToPath, { flags: "wx" });
          var size = 0;
          zlib.gzip(buffer, (error, result) => {
            try {
              result.forEach((b, index) => {
                if (index && !(index % this.options.bytesPerLine)) {
                  writeStream.write("\n");
                }
                writeStream.write("0x" + ("00" + b.toString(16).toUpperCase()).substr(-2) + ",");
                size++;
              });
            } finally {
              writeStream.end();
            }
          });

          fileInfo.push({
            path: relativeFilePath.replace(sep, "/"),
            variable,
            size,
            mimeType
          });
        };

        const processFiles = () => {
          // remove existing files
          for (const filePath of getFilesSync(wwwPath)) {
            unlinkSync(filePath);
          }

          // copy static data
          for (const filePath of getFilesSync(buildPath)) {
            const readStream = readFileSync(filePath);
            const relativeFilePath = relative(buildPath, filePath);
            processFile(relativeFilePath, readStream);
          }

          // copy assets
          const { assets } = compilation;
          Object.keys(assets).forEach((relativeFilePath) => {
            processFile(relativeFilePath, coherseToBuffer(assets[relativeFilePath].source()));
          });
        }

        const writeRegistrationRoutes = (writeStream) => {
          console.log(JSON.stringify(fileInfo, 2, 2));
        }

        // process the files
        processFiles();

        // write registration routes
        writeRegistrationRoutes();

        // trigger the next plugin
        callback();
      }
    );
  }
}

module.exports = ProgmemGenerator;
