/// <reference types="vite/client" />

interface ImportMetaEnv {
  readonly VITE_PROJECT_NAME: string;
  readonly VITE_PROJECT_PATH: string;
}

interface ImportMeta {
  readonly env: ImportMetaEnv;
}
