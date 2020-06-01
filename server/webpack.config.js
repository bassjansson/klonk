const path = require('path')
const HtmlWebpackPlugin = require('html-webpack-plugin')

const srcPath = path.resolve(__dirname, 'src')
const publicPath = path.resolve(__dirname, 'public')
const appPath = path.resolve(srcPath, process.env.APP || 'app')

const DEV = process.env.DEV

module.exports = {
    mode: DEV ? 'development' : 'production',

    entry: path.resolve(appPath, 'index.jsx'),

    output: {
        path: publicPath,
        filename: 'app.bundle.js',
        publicPath: '/'
    },

    devtool: DEV ? 'cheap-module-eval-source-map' : '',

    devServer: {
        contentBase: publicPath,
        port: 8080,
        hot: true
    },

    resolve: {
        extensions: [
            '.js', '.jsx', '.ts', '.tsx',
            '.css', '.scss',
            '.json'
        ]
    },

    module: {
        rules: [{
                test: /\.(js|ts)x?$/,
                exclude: /node_modules/,
                use: ['babel-loader']
            },
            {
                test: /\.(s?)css$/,
                use: ['style-loader', 'css-loader', 'sass-loader']
            }
        ]
    },

    plugins: [
        new HtmlWebpackPlugin({
            filename: 'index.html',
            template: path.resolve(appPath, `index-${DEV ? 'dev' : 'prod'}.html`),
            hash: true
        })
    ],

    externals: DEV ? {} : {
        'react': 'React',
        'react-dom': 'ReactDOM',
        'react-bootstrap': 'ReactBootstrap'
    }
}
