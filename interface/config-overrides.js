const CompressionPlugin = require("compression-webpack-plugin");
const ManifestPlugin = require('webpack-manifest-plugin');
const SWPrecacheWebpackPlugin = require('sw-precache-webpack-plugin');
const ExtractTextPlugin = require('extract-text-webpack-plugin');

const path = require('path');
const fs = require('fs');

module.exports = function override(config, env) {
  if (env === "production") {
    // rename the ouput file, we need it's path to be short, for SPIFFS
    config.output.filename = 'js/[name].[chunkhash:4].js';

    // disable sourcemap for production build
    config.devtool = false;

    // take out the manifest and service worker
    config.plugins = config.plugins.filter(plugin => !(plugin instanceof ManifestPlugin));
    config.plugins = config.plugins.filter(plugin => !(plugin instanceof SWPrecacheWebpackPlugin));

    const extractTextPlugin = config.plugins.find((plugin) => plugin instanceof ExtractTextPlugin);
    extractTextPlugin.filename = "css/[name].[contenthash:4].css";

    // add compression plugin, compress javascript, html and css
    config.plugins.push(new CompressionPlugin({
      asset: "[path].gz[query]",
      algorithm: "gzip",
      test: /\.(js|html|css)$/,
      deleteOriginalAssets: true
    }));
  }
  return config;
}
