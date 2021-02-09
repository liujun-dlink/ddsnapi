// module.exports.ddsnapi = require("./build/Release/ddsnapi");
const addon = require("./build/Release/ddsnapi");

exports.getPerformance = function(callback){
  addon.getPerformance(callback);
}

exports.getPerformanceSync = function(callback){
  addon.getPerformanceSync(callback);
}