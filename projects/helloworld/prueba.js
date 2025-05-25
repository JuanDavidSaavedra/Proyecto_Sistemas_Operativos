const http = require('http');
const path = require('path');
const fs = require('fs');

const server = http.createServer((req, res) => {
    let filePath = path.join(__dirname, req.url === '/' ? 'index.html' : req.url);
    let file_extname = path.extname(filePath);
    let contentType = 'text/html';

    switch(file_extname){
        case '.js':
            contentType = 'text/javascript';
            break;
        case '.css':
            contentType = 'text/css';
            break;
        case '.json':
            contentType = 'application/json';
            break;
        case '.png':
            contentType = 'image/png';
            break;
        case '.JPG':
            contentType = 'image/jpg';
            break;
        case '.ico':
            filePath = path.join(__dirname,'favicon.png');
            contentType = 'image/png';
            break;
        case '.ttf':
            contentType = 'font/ttf';
            break;
        case '.woff':
            contentType = 'font/woff';
            break;
        case '.woff2':
            contentType = 'font/woff2';
            break;
    }
    // Read File
    fs.readFile(filePath, (err, content) => {
        if(err){
            if(err.code == 'ENOENT'){
                console.log('Page not found');
            }
            else{
                res.writeHead(500);
                res.end('Server Error: ${err.code}');
            }
        }
        else{
            res.writeHead(200, {'Content-Type': contentType});
            res.end(content);    
        }
    });
});

const PORT = process.env.PORT || 8080;

server.listen(PORT, () => console.log(`Server is running and port is ${PORT}`));