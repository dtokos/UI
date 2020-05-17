const mix = require('laravel-mix');
const webpack = require('webpack');

mix.webpackConfig({
	plugins: [
		new webpack.ProvidePlugin({
			'$': 'jquery',
			'jQuery': 'jquery',
			'window.jQuery': 'jquery',
		}),
	],
});

mix.js('src/js/bundle.js', 'dist/js/')
	.sass('src/scss/bundle.scss', 'dist/css/')
	.copy('src/index.html', 'dist/');
