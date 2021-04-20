const express = require('express');
const app = express();
const port = 3000;
const fs = require('fs')


const writeMultiPart = (req, res, parts) =>{
    if(parts && parts.length){
      console.log(parts[0]);
      fs.readFile(parts[0], 'utf8' , (err, data) => {
        if (err) {
          console.error(err)
          res.end(err);
          return
        }
        console.log(data)
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
      './src/main.html',
      './DevENV.html'
    ]
  );
});

app.get('/connect', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/connect.html',
      './DevENV.html'
    ]
  );
});

app.get('/settings', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/settings.html',
      './DevENV.html',
    ]
  );
});

app.get('/status', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/status.html',
      './DevENV.html',
    ]
  );
});

app.get('/hass', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/hass.html',
      './DevENV.html',
    ]
  );
});


app.get('/rest', (req, res) => {
  writeMultiPart(req,res,
    [
      './src/rest.html',
      './DevENV.html',
    ]
  );
});

app.get('/font.css', (req, res) => {
  res.sendfile("./src/font.css")
});

app.get('/styles.css', (req, res) => {
  res.sendfile("./src/styles.css")
});

app.get('/nes.css', (req, res) => {
  res.sendfile("./src/nes.css")
});

app.get('/highlight.js', (req, res) => {
  res.sendfile("./src/highlight.js")
});


app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
});


