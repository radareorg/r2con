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
     			if(strs){
                    //console.log(strs);
                    if(strs.length > 1 && strs[1].indexOf("0") === 0){
                        functs_names["0x" + strs[1]] = strs[0];                    
                    }else if(strs.length > 7 && strs[2] == "address:")
                        functs_length["0x" + strs[4]] = strs[7].substring(1);                        
     			}
     		}
		).on('pipe', function() {
 			 // all done
             //console.log(functs_names);
             //console.log(functs_length);

             for(k in functs_names){
                offset = parseInt(Number(k).toString(2),2);
                slength = functs_length[k];
                name = functs_names[k];
                
                if(slength && slength.indexOf("x") == 1 && offset > 0){
                    length = parseInt(slength);
                    if(length > 2){
                        //console.log("name: " + name + " off: " + offset + " len: " + length);
                        fs.seekSync(fd, offset, 0);
                        var buff = new Buffer(length);
                        fs.readSync(fd, buff, 0, length);
                        hex = buff.toString('hex');
                        console.log("zb " + name + " " + hex);
                    }
                }
             }

 			 console.log("zn -");
		});     	
});

