const addon = require("./build/Release/ddsnapi");


//测试开启标志位
var testGetUptimeAsync = false;
var testGetUptimeSync = false;
var testKillConsolesAsync = false;
var testKillConsolesSync = false;
var testUnTarAsync = false;
var testUnTarSync = false;
var testSystemAsync = false;
var testSystemSync = false;
var testSetPowerLedAsync = false;
var testSetPowerLedSync = false;
var testSetSsoLedAsync = false;
var testSetSsoLedSync = false;
var testGetFtpDataAsync = false;
var testGetFtpDataSync = false;
var testSetFtpDataAsync = false;
var testSetFtpDataSync = false;
var testGetHddFlagAsync = false;
var testGetHddFlagSync = false;
var testSetHddFlagAsync = false;
var testSetHddFlagSync = false;

var testGetPerformanceAsync = true;
var testGetPerformanceSync = false;

/**
 * 回调函数
 * @param {*} value 
//  */
// function callback(value)
// {
//     console.log("Callback Result: " + value);
// }

//-----------libUptime.so----------
//1.getUptime
if (testGetUptimeAsync)
{
    addon.getUptime(callback);
}
if (testGetUptimeSync)
{
    addon.getUptimeSync(callback);
}
//-----------libUptime.so----------


//-----------libKiller.so----------
//1.killConsoles
if (testKillConsolesAsync)
{
    addon.killConsoles(callback);
}
if (testKillConsolesSync)
{
    addon.killConsolesSync(callback);
}

//-----------libKiller.so----------


//-----------libKiller.so----------
//1.unTar
if (testUnTarAsync)
{
    addon.unTar("/root/codes/", "eventPub.tar.gz", "/root/codes/nodejs-so-test/all", callback);
}
if (testUnTarSync)
{
    addon.unTarSync("/root/codes/", "eventPub.tar.gz", "/root/codes/nodejs-so-test/all", callback);
}

//-----------libKiller.so----------


//-----------libSystem.so----------
//1/fotaStatus
if (testSystemAsync)
{
    console.log("Result: " + addon.getFotaStatus(callback));
}
if (testSystemSync)
{
    console.log("Result: " + addon.getFotaStatusSync(callback));
}
//-----------libSystem.so----------


//-----------libLed.so----------
if (testSetPowerLedAsync)
{
    console.log("Result: " + addon.setPowerLed(1, 1, callback));
}
if (testSetPowerLedSync)
{
    console.log("Result: " + addon.setPowerLedSync(1, 1, callback));
}
if (testSetSsoLedAsync)
{
    console.log("Result: " + addon.setSsoLed(1, callback));
}
if (testSetSsoLedSync)
{
    console.log("Result: " + addon.setSsoLedSync(1, callback));
}
//-----------libLed.so----------


//-----------libFtpSetting.so----------
if(testGetFtpDataAsync)
{
    console.log("Result: " + addon.getFtpData(callback));
}
if(testGetFtpDataSync)
{
    console.log("Result: " + addon.getFtpDataSync(callback));
}

if (testSetFtpDataAsync)
{
    console.log("Result: " + addon.setFtpData("172.18.192.201", 21, "admin", "admin", callback));
}
if (testSetFtpDataSync)
{
    console.log("Result: " + addon.setFtpDataSync("172.18.192.201", 21, "admin", "admin", callback));
}
//-----------libFtpSetting.so----------


//-----------libHdd.so----------
if (testGetHddFlagAsync) {
    console.log("Result: " + addon.getHddFlag(callback));
}
if (testGetHddFlagSync) {
    console.log("Result: " + addon.getHddFlagSync(callback));
}
if (testSetHddFlagAsync)
{
    console.log("Result: " + addon.setHddFlag(0, callback));
}
if (testSetHddFlagSync)
{
    console.log("Result: " + addon.setHddFlagSync(0, callback));
}
//-----------libHdd.so----------


// //-----------libCLI.so----------
// if (testGetPerformanceAsync)
// {
//     addon.getPerformance(callback);
// }
// if (testGetPerformanceSync)
// {
//     addon.getPerformanceSync(callback);
// }
// //-----------libCLI.so----------


exports.getPerformance = function(callback){
    addon.getPerformance(callback);
}

exports.getPerformanceSync = function(callback){
    addon.getPerformanceSync(callback);
}