var _cons_ = "";

var _cons_obj = function(){
  this.log = function(to_log){
    _cons_ += to_log + "\n";
  }
};

var console = new _cons_obj();

function execute(hi){
  console.log("FOREVER");
  while(true){}
}

