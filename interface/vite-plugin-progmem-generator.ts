import { resolve, relative, sep } from 'path';
import {
  readdirSync,
  existsSync,
  readFileSync,
  writeFileSync,
} from 'fs';
import zlib from 'zlib';
import mime from 'mime-types';
import type { Plugin } from 'vite';

const ARDUINO_INCLUDES = '#include <Arduino.h>\n\n';

interface ProgmemGeneratorOptions {
  outputPath: string;
  bytesPerLine?: number;
  indent?: string;
  includes?: string;
}

interface FileInfo {
  uri: string;
  mimeType: string;
  variable: string;
  size: number;
}

function getFilesSync(dir: string, files: string[] = []): string[] {
  if (!existsSync(dir)) {
    return files;
  }

  for (const entry of readdirSync(dir, { withFileTypes: true })) {
    const entryPath = resolve(dir, entry.name);
    if (entry.isDirectory()) {
      getFilesSync(entryPath, files);
    } else {
      files.push(entryPath);
    }
  }

  return files;
}

export function progmemGenerator(
  options: ProgmemGeneratorOptions
): Plugin {
  const {
    outputPath,
    bytesPerLine = 20,
    indent = '  ',
    includes = ARDUINO_INCLUDES,
  } = options;

  return {
    name: 'vite-plugin-progmem',
    apply: 'build',
    enforce: 'post',

    async closeBundle() {
      const buildPath = resolve(process.cwd(), 'dist');
      const absoluteOutputPath = resolve(process.cwd(), outputPath);

      const fileInfo: FileInfo[] = [];
      let outputContent = includes;

      const files = getFilesSync(buildPath);

      files.forEach((filePath) => {
        if (filePath === absoluteOutputPath) {
          return;
        }

        const relativeFilePath = relative(buildPath, filePath);
        const buffer = readFileSync(filePath);
        const variable = `ESP_REACT_DATA_${fileInfo.length}`;
        const mimeType = mime.lookup(relativeFilePath) || 'application/octet-stream';

        const zipBuffer = zlib.gzipSync(buffer);
        let size = 0;

        outputContent += `const uint8_t ${variable}[] PROGMEM = {`;

        for (const b of zipBuffer) {
          if (size % bytesPerLine === 0) {
            outputContent += `\n${indent}`;
          }
          outputContent +=
            '0x' +
            b.toString(16).toUpperCase().padStart(2, '0') +
            ',';
          size++;
        }

        outputContent += '\n};\n\n';

        fileInfo.push({
          uri: '/' + relativeFilePath.split(sep).join('/'),
          mimeType: String(mimeType),
          variable,
          size,
        });
      });

      // C++ class generation
      outputContent +=
        'typedef std::function<void(const String& uri, const String& contentType, const uint8_t * content, size_t len)> RouteRegistrationHandler;\n\n';

      outputContent += `class WWWData {\n${indent}public:\n${indent.repeat(
        2
      )}static void registerRoutes(RouteRegistrationHandler handler) {\n`;

      for (const file of fileInfo) {
        outputContent += `${indent.repeat(3)}handler("${file.uri}", "${file.mimeType}", ${file.variable}, ${file.size});\n`;
      }

      outputContent += `${indent.repeat(2)}}\n};\n`;

      writeFileSync(absoluteOutputPath, outputContent);
      console.log(
        `\n[progmem] Generated ${fileInfo.length} files into ${outputPath}`
      );
    },
  };
}
