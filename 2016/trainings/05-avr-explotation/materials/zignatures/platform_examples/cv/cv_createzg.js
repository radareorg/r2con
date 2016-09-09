var     lazy  = require("lazy"),
        fs    = require('fs-ext');
        _     = require('underscore');
         
if(process.argv.length < 5){	
	console.log("Usage: " + process.argv[0] + " " + process.argv[1] + " <namespace> <map_file> <bin_file>");
	return;	
}

var namespace = process.argv[2];
var map_file = process.argv[3];
var bin_file = process.argv[4];

var fd = fs.openSync(bin_file, 'r');

console.log("# Signatures");
console.log("zn " + namespace);

fs.flock(fd, 'ex', function (err) {
    if (err) {
        return console.log("Couldn't lock file");
    }
    else
		new lazy(fs.createReadStream(map_file))
    		.lines
     		.forEach(function(line){
     			strs = line.toString().match(/\S+/g);
     			if(strs && strs.length == 3 && strs[1].indexOf("x") === 1){
     				name = strs[0];
     				offset = parseInt(Number(strs[1]).toString(2),2) * 2;
     				length = parseInt(strs[2]) * 2;
     				//console.log("name: " + name + " off: " + offset + " len: " + length);     		     	
     				fs.seekSync(fd, offset, 0);
     				var buff = new Buffer(length);
     				fs.readSync(fd, buff, 0, length);
     				hex = buff.toString('hex');
     				console.log("zb " + name + " " + hex);
     			}
     		}
		).on('pipe', function() {
 			 // all done
 			 console.log("zn -");
		});     	
});

