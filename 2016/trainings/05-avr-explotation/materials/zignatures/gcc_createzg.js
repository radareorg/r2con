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

var functs_names = Array();
var functs_length = Array();

fs.flock(fd, 'ex', function (err) {
    if (err) {
        return console.log("Couldn't lock file");
    }
    else
		new lazy(fs.createReadStream(map_file))
    		.lines
     		.forEach(function(line){
     			strs = line.toString().match(/\S+/g);
     			if(strs && strs[0].indexOf("x0") === 1){
                    if(strs.length == 2)
                        functs_names[strs[0]] = strs[1];
                    else
                        functs_length[strs[0]] = strs[1];                        
     			}
     		}
		).on('pipe', function() {
 			 // all done
             for(k in functs_names){
                offset = parseInt(Number(k).toString(2),2);
                slength = functs_length[k];
                name = functs_names[k];
                
                if(slength && slength.indexOf("x") == 1 && offset > 0){
                    length = parseInt(slength);
                    //console.log("name: " + name + " off: " + offset + " len: " + length);
                    fs.seekSync(fd, offset, 0);
                    var buff = new Buffer(length);
                    fs.readSync(fd, buff, 0, length);
                    hex = buff.toString('hex');
                    console.log("zb " + name + " " + hex);
                }
             }

 			 console.log("zn -");
		});     	
});

