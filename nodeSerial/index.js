var express = require('express')
var bodyParser = require('body-parser');
var app = express()
var SerialPort = require('serialport').SerialPort;
var path    = require("path");


app.set('port', (process.env.PORT || 5000))
app.use(express.static(__dirname + '/public'))
app.use(express.static(__dirname ))
app.use(bodyParser.urlencoded({ extended: true }));


app.get('/', function(request, response) {
  response.sendFile(path.join(__dirname+'/touch.html'));
})

app.listen(app.get('port'), function() {
  console.log("Node app is running at localhost:" + app.get('port'))
})

app.post('/senddrawing', function(req, res) {
var port = new SerialPort('\\\\.\\COM3', {baudrate:9600},true);
//console.log(req.body.coords);
port.on('open', function() {
  var coords = req.body.coords;
  console.log(coords.length);
  var split_coords = [];
  var j = 0;

  for (var i = 0; i < coords.length; i += 32) {
    split_coords.push(coords.substring(i, Math.min(i + 32, coords.length)));
  }

  console.log(split_coords[0])

  //var coords = new Buffer("HimynameisAlishaandthisisastringHimynameisAlishaandthisisastringHimynameisAlishaandthisisastringendDONEHimynasdfasdfasdfasdfasdfasdfasdfasdfasdfameisAlishaEND");

  port.write(split_coords[j], function() {} );

  port.on('data', function(){
    console.log('got a star');
    if (j >= split_coords.length) return;
    j++;
    if (j === split_coords.length) {
      console.log('sent');
      port.close(function(){ console.log("closed")});
    }
    else {
      console.log(split_coords[j]);
      port.write(split_coords[j], function(){});
    }
  })

});


  res.send('Drawing sent: "' + req.body.coords + '".');

});
