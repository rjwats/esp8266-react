const { resolve, relative, sep } = require('path');
const { readdirSync, readFileSync, createWriteStream } = require('fs');
var zlib = require('zlib');
var mime = require('mime-types');

const ARDUINO_INCLUDES = "#include <Arduino.h>\n\n";

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

class ProgmemGenerator {

  constructor(options = {}) {
    const { outputPath, bytesPerLine = 20, indent = "  ", includes = ARDUINO_INCLUDES } = options;
    this.options = {
      outputPath,
      bytesPerLine,
      indent,
      includes
    };
  }

  apply(compiler) {
    compiler.hooks.emit.tapAsync(
      { name: 'ProgmemGenerator' },
      (compilation, callback) => {
        const fileInfo = [];
        const outputPath = resolve(compilation.options.context, this.options.outputPath);
        const writeStream = createWriteStream(outputPath, { flags: "w" });
        try {

          const writeIncludes = () => {
            writeStream.write(this.options.includes);
          }

          const processFile = (relativeFilePath, buffer) => {
            const variable = "ESP_REACT_DATA_" + fileInfo.length;
            const mimeType = mime.lookup(relativeFilePath);
            var size = 0;
            writeStream.write("const uint8_t " + variable + "[] PROGMEM = {");
            const zipBuffer = zlib.gzipSync(buffer);
            zipBuffer.forEach((b, index) => {
              if (!(index % this.options.bytesPerLine)) {
                writeStream.write("\n");
                writeStream.write(this.options.indent);
              }
              writeStream.write("0x" + ("00" + b.toString(16).toUpperCase()).substr(-2) + ",");
              size++;
            });
            writeStream.write("};\n\n");
            fileInfo.push({
              path: relativeFilePath.replace(sep, "/"),
              variable,
              size,
              mimeType
            });
          };

          const processFiles = () => {
            // process static files
            const buildPath = compilation.options.output.path;
            for (const filePath of getFilesSync(buildPath)) {
              const readStream = readFileSync(filePath);
              const relativeFilePath = relative(buildPath, filePath);
              processFile(relativeFilePath, readStream);
            }

            // process assets
            const { assets } = compilation;
            Object.keys(assets).forEach((relativeFilePath) => {
              processFile(relativeFilePath, coherseToBuffer(assets[relativeFilePath].source()));
            });
          }

          const writeRegistrationRoutes = (writeStream) => {
            console.log(JSON.stringify(fileInfo, 2, 2));
          }

          writeIncludes();
          processFiles();
          writeRegistrationRoutes();
          callback();

        } finally {
          writeStream.end();
        }
      }
    );
  }
}

module.exports = ProgmemGenerator;
