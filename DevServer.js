const express = require('express');
const app = express();
const port = 3000;
const fs = require('fs')


const writeMultiPart = (req, res, parts) =>{
    if(parts && parts.length){
      fs.readFile(parts[0], 'utf8' , (err, data) => {
        if (err) {
          console.error(err)
          res.end(err);
          return
        }
        res.write(data, 'utf8', () => {
          parts.splice(0,1);
          writeMultiPart(req,res,parts)
        })
      });
    } else {
      res.end('');
    }
};


app.get('/', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/web-interface/main.html',
      './DevENV.html'
    ]
  );
});

app.get('/connect', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/web-interface/connect.html',
      './DevENV.html'
    ]
  );
});


app.get('/hass', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/web-interface/hass.html',
      './DevENV.html',
    ]
  );
});


app.get('/rest', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/web-interface/rest.html',
      './DevENV.html',
    ]
  );
});

app.get('/font.css', (req, res) => {
  res.sendfile("./src/web-interface/font.css")
});

app.get('/styles.css', (req, res) => {
  res.sendfile("./src/web-interface/styles.css")
});

app.get('/nes.css', (req, res) => {
  res.sendfile("./src/web-interface/nes.css")
});

app.get('/highlight.js', (req, res) => {
  res.sendfile("./src/web-interface/highlight.js")
});


app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
});


