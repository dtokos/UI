const mix = require('laravel-mix');

mix.js('src/js/bundle.js', 'dist/js/')
	.sass('src/scss/bundle.scss', 'dist/css/')
	.copy('src/index.html', 'dist/');
