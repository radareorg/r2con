net = require('net');

var buffer = [];
var spawn    = require('child_process').spawn,
    simulavr = spawn('simulavr', ['-d', 'atmega128', '-f', '/home/radare/workshop/ex3.4_simulator/build-crumbuino128/ex3.4_simulator.elf', 
			'-F', '16000000', '-x', '-,E1,9600', '-y', '-,E0,9600', '-z', '/home/radare/workshop/ex3.4_simulator/serial1.txt,D3,9600']);
var clients = [];
var currclient = 0;


if (!String.prototype.startsWith) {
  String.prototype.startsWith = function(searchString, position) {
    position = position || 0;
    return this.indexOf(searchString, position) === position;
  };
}


parseCmd = function (buf) {
    var str = buf.join("");

    if(str.startsWith("AT+CIPSEND=")){
        currclient = parseInt((str.substr(11).split(","))[0]);
        simulavr.stdout.write("OK\r\n");
        console.log("recieving data...");
    }else if(str.startsWith("AT+CIPCLOSE=")){
        currclient = parseInt((str.substr(12).split(",")));
        if(clients[currclient]){
            clients[currclient].end();
            clients.splice(currclient, 1);
        }
        simulavr.stdout.write("OK\r\n");
        console.log("closing...");
    }else if(str.startsWith("AT")){
        simulavr.stdout.write("OK\r\n");
    }else{
        if(clients.length > 0)
            clients[currclient].write(str);
    }

    console.log(str);
}

simulavr.stdin.setEncoding('ascii');
simulavr.stdout.setEncoding('ascii');
simulavr.stderr.setEncoding('ascii');

simulavr.stderr.on('data', function (data) {
  process.stdout.write(data);
});

simulavr.on('close', function (code) {
  console.log('terminated: ' + code);
});

simulavr.stdout.on('data', function (data) {
  simulavr.stdout.write(data);
  buffer.push(data);

  if(data == '\n'){
    parseCmd(buffer);
    buffer = [];
  }
});

net.createServer(function (socket) {

  socket.name = socket.remoteAddress + ":" + socket.remotePort

  clients.push(socket);

  socket.on('data', function (data) {
    currclient = clients.indexOf(socket);
    console.log('+IPD,' + currclient + ',' + data.length + ':');
    console.log(data.toString());
    simulavr.stdin.write('+IPD,' + currclient + ',' + data.length + ':');
    simulavr.stdin.write(data);
  });

  socket.on('end', function () {
    clients.splice(clients.indexOf(socket), 1);
  });

}).listen(5000);

console.log("Server running at port 5000\n");
