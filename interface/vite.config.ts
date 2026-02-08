import { defineConfig, CommonServerOptions } from 'vite';
import react from '@vitejs/plugin-react-swc';
import checker from 'vite-plugin-checker';
import svgr from "vite-plugin-svgr";
import pkg from './package.json';
import { progmemGenerator } from './vite-plugin-progmem-generator';

const target = process.env.PROXY || pkg.proxy;
const wsTarget = target.replace(/^http(s?):\/\//, "ws$1://");

const devProxyConfig: CommonServerOptions = {
  host: '0.0.0.0',
  open: true,
  port: 3000,
  strictPort: true,
  allowedHosts: true,
  proxy: {
    '/rest': {
      target: target,
      xfwd: true,
      changeOrigin: true,
    },
    '/ws': {
      target: wsTarget,
      ws: true,
      changeOrigin: true,
    }
  },
};

export default defineConfig({
  plugins: [
    react(),
    checker({
      typescript: { tsconfigPath: 'tsconfig.app.json' },
      eslint: {
        lintCommand: 'eslint --max-warnings 0 "./src/**/*.{ts,tsx}"',
        useFlatConfig: true
      }
    }),
    svgr(),
    progmemGenerator({
      outputPath: "../lib/framework/WWWData.h",
      bytesPerLine: 20
    })
  ],
  server: devProxyConfig,
  preview: devProxyConfig,
  define: {
    global: 'globalThis',
  },
  esbuild: {
    legalComments: 'none',
  },
  build: {
    chunkSizeWarningLimit: 1024,
    sourcemap: false,
    rollupOptions: {
      output: {
        entryFileNames: 'js/[name].[hash:6].js',
        chunkFileNames: 'js/[name].[hash:6].js',
        assetFileNames: (assetInfo: any) => {
          if (assetInfo.name?.endsWith('.css')) {
            return 'css/[name].[hash:6].css';
          }
          return 'assets/[name].[hash:6][extname]';
        },
      },
    },
  }
});
