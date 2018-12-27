// JavaScript Document
//Popup function
function popUp(URL) {
	day = new Date();
	id = day.getTime();
	eval("page" + id + " = window.open(URL, '" + id + "', 'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=0,width=827,height=662');");
}

function appendTableRow(table, align, content){
    var tr = table.insertRow(-1);
    var i;
    for(i=0; i < content.length; i++){
        var td = tr.insertCell(-1);
        td.align = align;
        td.innerHTML = content[i];
    }
}

function insertTableRow(table, index, align, content){
    var tr = table.insertRow(index);
    var i;
    for(i=0; i < content.length; i++){
        var td = tr.insertCell(-1);
        td.align = align;
        td.innerHTML = content[i];
    }
}
//show/hide steps
function showHideSteps(hideThis, showThis){
	var hideThis = document.getElementById(hideThis);
	var showThis =document.getElementById(showThis);
	hideThis.style.display ='none';
	showThis.style.display ='block';
}
var vis=1;
function cover(me) {
	var cover=document.getElementById(me);
	if(vis){
		vis=0;
		cover.style.display='block';      
    }
	else 
	{   
		vis=1;
		cover.style.display='none'; 
    }
}

function appendTableEmptyRow(table, align, colspan, content){
    var tr = table.insertRow(-1);
    var td = tr.insertCell(-1);
    if (isNaN(colspan))
        colspan = 1;
    td.align = align;
    td.colSpan = colspan;
    td.innerHTML = content;
}

function get_defaultKeyFlagArray(flagArray, flag)
{
    for (var i=0; i<4; i++)
	flagArray[i] = flag >> i & 1;
}

function set_defaultKeyFlag(flag, idx, sts)
{
    var ret;
    if (sts == "1")
    ret = flag | 1 << idx;
    else {
	var temp = 1;
	var sum = 0;
	for (var i=0; i<4; i++) {
	    if (idx != i)
		sum += temp;
	    temp = temp * 2;
	}
	ret = flag & sum;
    }
    return ret;
}

/* modify thd "idx" bit of "sStr" to "bit" */
function modifyBit(sStr, idx, bit)
{
    var idx_num=parseInt(idx, 10);
    var from_pat = new RegExp("(.{" + idx_num + "}).");
    var to_pat = "$1" + bit;
    return sStr.replace(from_pat, to_pat);
}

function rtrim(str)
{
	var length ;
	length = str.length;
	if(str.charAt(length-1)==" "){
		str = str.slice(0,length-1);
		str = rtrim(str);
	}
	return str;
}

function ltrim(str)
{

	if(str.charAt(0)==" "){
		str = str.slice(1);
		str = ltrim(str);
	}
	return str;
}

function trim(str)
{
	return ltrim(rtrim(str));
}
