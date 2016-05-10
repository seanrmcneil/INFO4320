var express = require('express')
var bodyParser = require('body-parser');
var app = express()
var SerialPort = require('serialport').SerialPort;


app.set('port', (process.env.PORT || 5000))
app.use(express.static(__dirname + '/public'))
app.use(bodyParser.urlencoded({ extended: true })); 


html = '<!DOCTYPE html><html lang="en"><head> <meta charset="utf-8"> <meta http-equiv="X-UA-Compatible" content="IE=edge"> <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0"> <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script></head><style>.submit{background-color: #008080; font-size: 14pt; width: 100px; padding: 10px; text-align: center; cursor: pointer;}</style><body> <script type="text/javascript">function initTouchCanvas(){var datadiv=document.getElementById("datadiv"); datadiv.style.display="none"; var canvas=document.getElementById("mycanvas"); canvas.style.display="block"; canvas.addEventListener("touchstart", draw, false); canvas.addEventListener("mousedown", drawMouse, false);}function draw(event){var canvas=document.getElementById("mycanvas"); var offset=getOffset(canvas); var datadiv=document.getElementById("datadiv"); if (canvas.getContext){var ctx=canvas.getContext("2d"); ctx.fillStyle="#ff0000"; ctx.fillRect(event.touches[0].pageX - offset.left, event.touches[0].pageY - offset.top, 5, 5);}}function drawMouse(event){var canvas=document.getElementById("mycanvas"); var offset=getOffset(canvas); var datadiv=document.getElementById("datadiv"); if (canvas.getContext){var ctx=canvas.getContext("2d"); ctx.fillStyle="#ff0000"; ctx.fillRect(event.pageX - offset.left, event.pageY - offset.top, 5, 5);}}function getOffset(obj){var offsetLeft=0; var offsetTop=0; do{if (!isNaN(obj.offsetLeft)){offsetLeft +=obj.offsetLeft;}if (!isNaN(obj.offsetTop)){offsetTop +=obj.offsetTop;}}while (obj=obj.offsetParent); return{left: offsetLeft, top: offsetTop};}</script> <script type="text/javascript"></script> <p> <canvas id="touchmovezone" width="510" height="660" style="border: 1px solid #ccc;display: block"> <br/>Canvas element not supported <br/> </canvas> <br/> <br/> </p><form action="/senddrawing" method="post"> <input type="hidden" value="yo" id="coords" name="coords"/> <input type="submit" value="Send Drawing"/> </form> </form></body><script>(function(){initTouchMoveCanvas(); var touchmovezone; var touchmovectx; var lastPt=null; var x_coords=[]; var y_coords=[]; function initTouchMoveCanvas(){touchmovezone=document.getElementById("touchmovezone"); touchmovezone.style.display="block"; touchmovezone.addEventListener("touchmove", drawtouchmove, false); touchmovezone.addEventListener("touchend", endtouchmove, false); touchmovezone.addEventListener("mousedown", function(){touchmovezone.addEventListener("mousemove", drawmousemove, false);}, false); touchmovezone.addEventListener("mouseup", endmousemove, false); touchmovectx=touchmovezone.getContext("2d");}function drawtouchmove(e){e.preventDefault(); var offset=getOffset(touchmovezone); if (lastPt !=null){touchmovectx.beginPath(); touchmovectx.moveTo(lastPt.x - offset.left, lastPt.y - offset.top); touchmovectx.lineTo(e.touches[0].pageX - offset.left, e.touches[0].pageY - offset.top); touchmovectx.strokeStyle="purple"; touchmovectx.lineWidth=3; touchmovectx.stroke();}lastPt={x: e.touches[0].pageX, y: e.touches[0].pageY};}function drawmousemove(e){e.preventDefault(); var offset=getOffset(touchmovezone); if (lastPt !=null){touchmovectx.beginPath(); touchmovectx.moveTo(lastPt.x - offset.left, lastPt.y - offset.top); touchmovectx.lineTo(e.pageX - offset.left, e.pageY - offset.top); touchmovectx.strokeStyle="purple"; touchmovectx.lineWidth=3; touchmovectx.stroke();}lastPt={x: e.pageX, y: e.pageY}; x_coords.push(e.pageX); y_coords.push(e.pageY);}function endtouchmove(e){e.preventDefault(); lastPt=null;}function endmousemove(e){e.preventDefault(); touchmovezone.removeEventListener("mousemove", drawmousemove, false); lastPt=null; x_coords.push("u"); y_coords.push("u"); document.getElementById("coords").value=("x"+x_coords.toString() + ",y" + y_coords.toString() +",e");}})();</script></html>'
app.get('/', function(request, response) {
  response.send(html);
})

app.listen(app.get('port'), function() {
  console.log("Node app is running at localhost:" + app.get('port'))
})

app.post('/senddrawing', function(req, res) {


var port = new SerialPort('/dev/cu.usbmodem1411');
//console.log(req.body.coords);
port.on('open', function() {
  var coords = req.body.coords;
  console.log(coords.length);
  var split_coords = [];
  var j = 0;

  for (var i = 0; i < coords.length; i += 62) {
    split_coords.push(coords.substring(i, Math.min(i + 62, coords.length)));
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
    }
    else {
      console.log(split_coords[j]);
      port.write(split_coords[j], function(){});
    }
  })

});

  res.send('Drawing sent: "' + req.body.coords + '".');
});
