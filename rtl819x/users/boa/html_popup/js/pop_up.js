/*********************************************************************
 * pop_up.js
 * category   javascript
 * package    jquery
 * author     youyou   
 **********************************************************************/ 
(function($) {
	var cachedata = {};
	var arrweebox = new Array();
	var tag;
	var weebox = function(content,options) {
		var self 		= this;
		this.dh 		= null;
		this.mh 		= null;
		this.dc			= null;
		this.dt			= null;
		this.db			= null;
		this.selector 	= null;	
		this.ajaxurl 	= null;
		this.options 	= null;
		this._dragging 	= false;
		this._content 	= content || '';
		this._options 	= options || {};
		this._defaults 	= {
			boxid: null,
			boxclass: null,
			cache: false,
			type: 'dialog',
			title: '',
			width: 0,
			height: 0,
			timeout: 0, 
			draggable: true,
			modal: true,
			focus: null,
			blur: null,
			position: 'center',
			overlay: 30,
			showTitle: true,
			showButton: true,
			showCancel: true, 
			showOk: true,
			okBtnName: 'Ok',
			cancelBtnName: 'Cancel',
			contentType: 'text',
			contentChange: false,
			clickClose: false,
			zIndex: 999,
			animate: '',
			showAnimate:'',
			hideAnimate:'',
			onclose: null,
			onopen: null,
			oncancel: null,
			onok: null,
			suggest:{url:'',tele:'',vele:'',fn:null},
			select:{url:'',type:'radio', tele:'',vele:'',width:120,search:false,fn:null}
		};
		//Initialization Options
		this.initOptions = function() {
			self._options = self._options || {};
			self._options.animate = self._options.animate || '';
			self._options.showAnimate = self._options.showAnimate || self._options.animate;
			self._options.hideAnimate = self._options.hideAnimate || self._options.animate;
			self._options.type = self._options.type || 'dialog';
			self._options.title = self._options.title || '';
			self._options.boxclass = self._options.boxclass || 'wee'+self._options.type;
			self._options.contentType = self._options.contentType || "";
			if (self._options.contentType == "") {
				self._options.contentType = (self._content.substr(0,1) == '#') ? 'selector' : 'text';
			}
			self.options  = $.extend({}, self._defaults, self._options);
			self._options = null;
			self._defaults = null;
		};
		//judge the browser version 
		function selectBrowser(){	
			var browser=navigator.appName 
			var b_version=navigator.appVersion 
			var version=b_version.split(";"); 
			if(browser=="Microsoft Internet Explorer"){
				var trim_Version=version[1].replace(/[ ]/g,""); 
				if(trim_Version=="MSIE7.0") 
				{ 
					return 7;
				} 
				else if(trim_Version=="MSIE6.0") 
				{ 
					return 6;
				}
				else if(trim_Version=="MSIE9.0") 
				{ 
					return 9;
				}				
			}
	
		}
		//Initialization pop box
		
		this.initBox = function() {
			var html = '';
			switch(self.options.type) {
				case 'alert':
				case 'select':
				case 'dialog':
					if(selectBrowser()==6){
						  html +='<div class="weedialog">';	
					}
					html += '<table  width="433" height="218" border="0" cellpadding="0" cellspacing="0">';
					html += '<tr>';
					html += '<td colspan="5" >';
					html += '<table border="0" cellpadding="0" cellspacing="0" width="433" class="dialog-header_new">';
					html += '<tr>';
					html += '<td style="cursor:pointer" class="dialog-tl_new" width="72">';
					html += '<img src="images/popup_01.png" width="72" height="74" alt="" >';
					html += '</td>';
					html += '<td height="74" colspan="2" background="images/popup_03.png"  width="320">';
					html += '<span class="dialog-title_new">CenturyLink Modem Configuration Help</span>';
					html += '</td>';
					html += '<td >';
					html += '<img src="images/popup_04.png" width="41" height="74" alt="">';
					html += '</td>';
					html += '</tr>';
					html += '</table>';
					html += '</td>';
					html += '</tr>';
					html += '<tr>';
					html += '<td colspan="5">';
					if(selectBrowser()==6){
						html += '<table width="433" height="65" border="0" cellpadding="0" cellspacing="0">';
					}else{
						html += '<table width="433" height="100" border="0" cellpadding="0" cellspacing="0">';
					}
					
					html += '<tr>';
					html += '<td width="20" height="12"><img src="images/table_01.png" width="20" height="12">';
					html += '</td>';
					html += '<td>';
					html += '<img src="images/table_02.png" width="388" height="12">';
					html += '</td>';
					html += '<td width="25">';
					html += '<img src="images/table_03.png" width="25" height="12">';
					html += '</td>';
					html += '</tr>';
					html += '<tr>';
					html += '<td background="images/table_04.png">&nbsp;</td>';
					html += '<td bgcolor="#124769">';
					html += '<DIV class = "div_content">';
					html += '<table border="0" cellpadding="2" cellspacing="1" class="TablePanel" width="95%" align="center" id="datatable" style=" overflow-y:scroll; overflow-x:hidden; table-layout: fixed;WORD-BREAK: keep-all; WORD-WRAP: break-word; text-align:center; color:#FFFFFF;" height="10px">';
					html += '<span class="dialog-content_new"></span>';
					html += '</table>';
					html += '</DIV>';
					html += '</td>';
					html += '<td background="images/table_06.png">&nbsp;</td>';
					html += '</tr>';
					html += '<tr>';
					html += '<td height="13">';
					html += '<img src="images/table_09.png" width="20" height="13">';
					html += '</td>';
					html += '<td>';
					html += '<img src="images/table_10.png" width="388" height="13">';
					html += '</td>';
					html += '<td>';
					html += '<img src="images/table_11.png" width="25" height="13">';
					html += '</td>';
					html += '</tr>';
					html += '</table>';
					html += '</td>';
					html += '</tr>';
					html += '<tr>';
					html += '<td>';
					html += '<img src="images/popup_09.png" width="37" height="44" alt="">';
					html += '</td>';
					html += '<td width="325" height="37" colspan="3" bgcolor="#1e71a1">';
					html += '<div class="dialog-button_new">';
					html += '<table width="186" height="25" border="0" align="right" cellpadding="0" cellspacing="0">';
					html += '<tr>';
					html += '<td align="right"><input type="button" class="dialog-ok_new" value="  Ok  "></td>';
					html += '<td align="right"><input type="button" class="dialog-cancel_new" value="Cancel"></td>';
					html += '</tr>';
					html += '</table>';
					html += '</div>'; 
					html += '</td>'; 
					html += '<td><img src="images/popup_11.png" width="41" height="44" alt=""></td>'; 
					html += '</tr>'; 
					html += '<tr>'; 
					html += '<td><img src="images/popup_12.png" width="37" height="42" alt=""></td>'; 
					html += '<td height="42" colspan="3" background="images/popup_14.png">'; 
					html += '<span class="popuplink" style="color:#fff;font-weight:bold;">Visit <a href="http://qwest.centurylink.com/internethelp/" target="_blank" class="hyperlink" style=" font-family:Arial, Helvetica, sans-serif; text-decoration: none; color:#fff;font-weight:bold;">http://qwest.centurylink.com/internethelp/</a> for details</span>'; 
					html += '</td>'; 
					html += '<td><img src="images/popup_15.png" width="41" height="42" alt=""></td>'; 
					html += '</tr>'; 
					html += '<tr>'; 
					html += '<td><img src="images/seperator.gif" width="37" height="1" alt=""></td>'; 
					html += '<td><img src="images/seperator.gif" width="35" height="1" alt=""></td>'; 
					html += '<td width="278"><img src="images/seperator.gif" width="278" height="1" alt=""></td>'; 
					html += '<td><img src="images/seperator.gif" width="42" height="1" alt=""></td>'; 
					html += '<td><img src="images/seperator.gif" width="11" height="1" alt=""></td>'; 
					html += '</tr>'; 
					html += '</table>'; 
					if(selectBrowser()==6){
						html +='</div>';
					}
					

						break;
				case 'custom':
				case 'suggest':
				html = '<div><div class="dialog-content_new"></div></div>';
						break;
			}
			self.dh = $(html).appendTo('body').hide().css({
				position: 'absolute',	
				overflow: 'hidden',
				zIndex: self.options.zIndex
				//bgcolor: "#000000"
			});
			self.div_content = self.find('.div_content');
			self.dc = self.find('.dialog-content_new');
			self.dt = self.find('.dialog-title_new');
			self.db = self.find('.dialog-button_new');
			if (self.options.boxid) {
				self.dh.attr('id', self.options.boxid);
			}	
			if (self.options.boxclass) {
				self.dh.addClass(self.options.boxclass);
			}
			if (self.options.height>0) {
				self.dc.css('height', self.options.height);
			}
			if (self.options.width>0) {
				self.dh.css('width', self.options.width);
			}
			if(selectBrowser()!=9){
				self.dh.bgiframe();	
			}
		}
		//Initialization Paste as mask
		this.initMask = function() {
			if(selectBrowser()==9){
				if (self.options.modal) {
					self.mh = $("<div class='dialog-mask'></div>")
					.appendTo('body').hide().css({
						width: self.bwidth(),
						height: self.bheight(),
						zIndex: self.options.zIndex-1
					});
				}
			}else{
				if (self.options.modal) {
					self.mh = $("<div class='dialog-mask'></div>")
					.appendTo('body').hide().css({
						width: self.bwidth(),
						height: self.bheight(),
						zIndex: self.options.zIndex-1
					}).bgiframe();
				}
			}
			
		}
		//Initialization the contents of pop
		this.initContent = function(content) {
			self.dh.find(".dialog-ok_new").val(self.options.okBtnName);
			self.dh.find(".dialog-cancel_new").val(self.options.cancelBtnName);	
			if (self.options.title == '') {
				//self.dt.hide();	
				//self.dt.html(self._titles[self._options.type] || '');
			} else {
				self.dt.html(self.options.title);
			}
			if (!self.options.showTitle) {
				self.dh.find('.dialog-header_new').hide();
			}	
			if (!self.options.showButton) {
				self.dh.find('.dialog-button_new').hide();
			}
			if (!self.options.showCancel) {
				self.dh.find('.dialog-cancel_new').hide();
			}							
			if (!self.options.showOk) {
				self.dh.find(".dialog-ok_new").hide();
			}
			if (self.options.type == 'suggest') {
				self.hide();
				$(self.options.suggest.tele).unbind('keyup').keyup(function(){
					var val = $.trim(this.value);
					var data = null;
					for(key in cachedata) {
						if (key == val) {
							data = cachedata[key];
						}
					}
					if (data === null) {
						$.ajax({
							type: "GET",
							data:{q:val},
						  	url: self.options.suggest.url || self._content,
						  	success: function(res){data = res;},
						  	dataType:'json',
						  	async: false				  	
						});
					}
					cachedata[val] = data;
					var html = '';
					for(key in data) {
						html += '<li>'+data[key].name+'</li>';
					}
					self.setContent(html);
					self.show();
					self.find('li').click(function(){
						var i = self.find('li').index(this);
						$(self.options.suggest.tele).val(data[i].name);
						$(self.options.suggest.vele).val(data[i].id);
						if (typeof self.options.suggest.fn == 'function') {
							fn(data[i]);
						}
						self.hide();
					});
				});
			} else if(self.options.type == 'select') {
				var type = self.options.select.type || 'radio';
				var url = self.options.select.url || self._content || '';
				var search = self.options.select.search || false;
				$.ajax({
					type: "GET",
				  	url: url,
				  	success: function(data){
						var html = '';
						if (data === null) {
							html = 'no data';
						} else {
							if (search) {
								html += '<div class="wsearch"><input type="text"><input type="button" value="Query"></div>';
							}
							var ovals = $.trim($(self.options.select.vele).val()).split(',');
							html += '<div class="wselect">';
							for(key in data) {
								var checked = ($.inArray(data[key].id, ovals)==-1)?'':'checked="checked"'; 
								html += '<li><label><input name="wchoose" '+checked+' type="'+type+'" value="'+data[key].id+'">'+data[key].name+'</label></li>';
							}
							html += '</div>';
						}
						self.setContent(html);
						self.find('li').width(self.options.select.width);
						self.find('.wsearch input').keyup(function(){
							var v = $.trim(this.value);
							self.find('li').hide();
							for(i in data) {
								if (data[i].id==v || data[i].name.indexOf(v)!=-1) {
									self.find('li:eq('+i+')').show();
								}
							}
						});
						self.setOnok(function(){
							if (type=='radio') {
								if (self.find(':checked').length == 0) {
									$(self.options.select.tele).val('');
									$(self.options.select.vele).val('');
								} else {
									var i = self.find(':radio[name=wchoose]').index(self.find(':checked')[0]);
									$(self.options.select.tele).val(data[i].name);
									$(self.options.select.vele).val(data[i].id);
									if (typeof self.options.select.fn == 'function') fn(data[i]);
								}
							} else {
								if (self.find(':checked').length == 0) {
									$(self.options.select.tele).val('');
									$(self.options.select.vele).val('');
								} else {
									var temps=[],ids=[],names=[];
									self.find(':checked').each(function(){
										var i = self.find(':checkbox[name=wchoose]').index(this);
										temps.push(data[i]);
										ids.push(data[i].id);
										names.push(data[i].name);
									});
									$(self.options.select.tele).val(names.join(","));
									$(self.options.select.vele).val(ids.join(","));
									if (typeof self.options.select.fn == 'function') fn(temps);
								}
							}
							self.close();
						});
						self.show();
					},
				  	dataType:'json'
				});
			} else {				
				if (self.options.contentType == "selector") {
					self.selector = self._content;
					self._content = $(self.selector).html();
					self.setContent(self._content);
					//if have checkbox do
					var cs = $(self.selector).find(':checkbox');
					self.dc.find(':checkbox').each(function(i){
						this.checked = cs[i].checked;
					});
					$(self.selector).empty();
					self.show();
					self.focus();
					self.onopen();
				} else if (self.options.contentType == "ajax") {	
					self.ajaxurl = self._content;	
					self.setLoading();				
					self.show();
					self.dh.find(".dialog-button_new").hide();
					if (self.options.cache == false) {
						if (self.ajaxurl.indexOf('?') == -1) {
							self.ajaxurl += "?_t="+Math.random();
						} else {
							self.ajaxurl += "&_t="+Math.random();
						}
					}
					$.get(self.ajaxurl, function(data) {
						self._content = data;
				    	self.setContent(self._content);
				    	self.show();
						self.focus();
				    	self.onopen();
					});
				} else {
					self.setContent(self._content);	
					self.show();
				//	self.focus();
					self.onopen();
				}
			}
		}
		//Initialization pop event
		this.initEvent = function() {
			self.dh.find(".dialog-close, .dialog-cancel_new, .dialog-ok_new").unbind('click').click(function(){self.close()});			
			if (typeof(self.options.onok) == "function") {
				self.dh.find(".dialog-ok_new").unbind('click').click(function(){self.options.onok(self);tag = "1";});
			} 
			if (typeof(self.options.oncancel) == "function") {
				self.dh.find(".dialog-cancel_new").unbind('click').click(function(){self.options.oncancel(self);tag = "2"});
			}	
			if (self.options.timeout>0) {
				window.setTimeout(self.close, (self.options.timeout * 1000));
			}

			self.dh.find(".dialog-tl_new").unbind('click').click(function(){
				day = new Date();
				id = day.getTime();
				var URL = 'help.html';
				eval("page" + id + " = window.open(URL, '" + id + "', 'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=0,width=827,height=662');");
			});			
			this.drag();
		}
		//Set onok event
		this.setOnok = function(fn) {
			self.dh.find(".dialog-ok_new").unbind('click');
			if (typeof(fn)=="function")	self.dh.find(".dialog-ok_new").click(function(){fn(self)});
		}
		//Set onOncancel event
		this.setOncancel = function(fn) {
			self.dh.find(".dialog-cancel_new").unbind('click');
			if (typeof(fn)=="function")	self.dh.find(".dialog-cancel_new").click(function(){fn(self)});
		}
		//Set onOnclose event
		this.setOnclose = function(fn) {
			self.options.onclose = fn;
		}
		//Pop drag
		this.drag = function() {		
			if (self.options.draggable && self.options.showTitle) {
				self.dh.find('.dialog-header_new').mousedown(function(event){
					var h  = this; 
					var o  = document;
					var ox = self.dh.position().left;
					var oy = self.dh.position().top;
					var mx = event.clientX;
					var my = event.clientY;
					var width = self.dh.width();
					var height = self.dh.height();
					var bwidth = self.bwidth();
					var bheight = self.bheight(); 
			        if(h.setCapture) {
			            h.setCapture();
			        }
					$(document).mousemove(function(event){						
						if (window.getSelection) {
							window.getSelection().removeAllRanges();
						} else { 
				        	document.selection.empty();
				        }
						//TODO
						var left = Math.max(ox+event.clientX-mx, 0);
						var top = Math.max(oy+event.clientY-my, 0);
						var left = Math.min(left, bwidth-width);
						var top = Math.min(top, bheight-height);
						self.dh.css({left: left, top: top});
					}).mouseup(function(){
						if(h.releaseCapture) { 
			                h.releaseCapture();
			            }
				        $(document).unbind('mousemove');
				        $(document).unbind('mouseup');
					});
				});			
			}	
		}
		//Open the pre-callback function
		this.onopen = function() {							
			if (typeof(self.options.onopen) == "function") {
				self.options.onopen(self);
				tag = "0";
			}	
		}
		//Add a button
		this.addButton = function(opt) {
			opt = opt || {};
			opt.title = opt.title || 'OK';
			opt.bclass = opt.bclass || 'dialog-btn1';
			opt.fn = opt.fn || null;
			opt.index = opt.index || 0;
			var btn = $('<input type="button" class="'+opt.bclass+'" value="'+opt.title+'">').click(function(){
				if (typeof opt.fn == "function") opt.fn(self);
			});
			if (opt.index < self.db.find('input').length) {
				self.db.find('input:eq('+opt.index+')').before(btn);
			} else {
				self.db.append(opt);
			}			
		}
		//Display pop
		this.show = function() {
			if (self.options.showButton) {
				self.dh.find('.dialog-button_new').show();
			}
			if (self.options.position == 'center') {
				self.setCenterPosition();
			} else {
				self.setElementPosition();
			}
			if (typeof self.options.showAnimate == "string") {
				self.dh.show(self.options.animate);
			} else {
				self.dh.animate(self.options.showAnimate.animate, self.options.showAnimate.speed);
			}
			if (self.mh) {
				self.mh.show();
			}
		}
		this.hide = function(fn) {
			if (typeof self.options.hideAnimate == "string") {
				self.dh.hide(self.options.animate, fn);
			} else {
				self.dh.animate(self.options.hideAnimate.animate, self.options.hideAnimate.speed, "", fn);
			}
		}
		//Set the focus of pop
		this.focus = function() {
			if (self.options.focus) {
				self.dh.find(self.options.focus).focus();
			} else {
				self.dh.find('.dialog-cancel_new').focus();
			}
		}
		//Find element in pop
		this.find = function(selector) {
			return self.dh.find(selector);
		}
		//Setting load state
		this.setLoading = function() {			
			self.setContent('<div class="dialog-loading"></div>');
			self.dh.find(".dialog-button_new").hide();
			if (self.dc.height()<90) {				
				self.dc.height(Math.max(90, self.options.height));
			}
			if (self.dh.width()<200) {
				self.dh.width(Math.max(200, self.options.width));
			}
		}
		this.setWidth = function(width) {
			self.dh.width(width);
		}
		//Set title
		this.setTitle = function(title) {
			self.dt.html(title);
		}
		//Get title
		this.getTitle = function() {
			return self.dt.html();
		}
		//Set content
		this.setContent = function(content) {
			var content_len = content.length;
			var showLen = content_len *0.5;
			self.dc.html(content);
			if (self.options.height>0) {
				self.dc.css('height', self.options.height);
			} else {
				self.dc.css('height','');
			}
			
			//the height largen by the connect,when the height>300,will show scrollbar
		
			if (self.options.width>0) {
				self.dh.css('width', self.options.width);
				if(showLen>=300){
					self.dh.css('height',300+185);
					self.div_content.css('height',300);
				}else if(showLen>50 && showLen<300){
					self.dh.css('height',showLen+185);
					self.div_content.css('height',showLen+2);
				}else if(showLen>0 &&showLen<=50){
					self.dh.css('height',260);
					if(selectBrowser()==6){
						self.div_content.css('height',60);
					}else{
						self.div_content.css('height',40);	
					}
				}
				
			} else {
				self.dh.css('width','');
			}
			if (self.options.showButton) {
				self.dh.find(".dialog-button_new").show();
			}
		}
		//Get content
		this.getContent = function() {
			return self.dc.html();
		}	
		
		this.disabledButton = function(btname, state) {
			self.dh.find('.dialog-'+btname).attr("disabled", state);
		}
		//Hide button
		this.hideButton = function(btname) {
			self.dh.find('.dialog-'+btname).hide();			
		}
		//show button
		this.showButton = function(btname) {
			self.dh.find('.dialog-'+btname).show();	
		}
		//Set the button title
		this.setButtonTitle = function(btname, title) {
			self.dh.find('.dialog-'+btname).val(title);	
		}
		//Operation is complete
		this.next = function(opt) {
			opt = opt || {};
			opt.title = opt.title || self.getTitle();
			opt.content = opt.content || "";
			opt.okname = opt.okname || "ok";
			opt.width = opt.width || 260;
			opt.onok = opt.onok || self.close;
			opt.onclose = opt.onclose || null;
			opt.oncancel = opt.oncancel || null;
			opt.hideCancel = opt.hideCancel || true;
			self.setTitle(opt.title);
			self.setButtonTitle("ok", okname);
			self.setWidth(width);
			self.setOnok(opt.onok);
			if (opt.content != "") self.setContent(opt.content);
			if (opt.hideCancel)	self.hideButton("Cancel");
			if (typeof(opt.onclose) == "function") self.setOnclose(opt.onclose);
			if (typeof(opt.oncancel) == "function") self.setOncancel(opt.oncancel);
			self.show();
		}
		//Close pop
		this.close = function(n) {
			if (typeof(self.options.onclose) == "function") {
				self.options.onclose(self);
			}
			if (self.options.contentType == 'selector') {
				if (self.options.contentChange) {
					//if have checkbox do
					var cs = self.find(':checkbox');
					$(self.selector).html(self.getContent());						
					if (cs.length > 0) {
						$(self.selector).find(':checkbox').each(function(i){
							this.checked = cs[i].checked;
						});
					}
				} else {
					$(self.selector).html(self._content);
				}
			}
			//Setting the focus after close
			if (self.options.blur) {
				$(self.options.blur).focus();
			}
			
			for(i=0;i<arrweebox.length;i++) {
				if (arrweebox[i].dh.get(0) == self.dh.get(0)) {
					arrweebox.splice(i, 1);
					break;
				}
			}
			self.dh.slideUp(10);
			if (self.mh) {
				self.mh.remove();
			}
		}
		
		this.bheight = function() {
			if ($.browser.msie && $.browser.version < 7) {
				var scrollHeight = Math.max(
					document.documentElement.scrollHeight,
					document.body.scrollHeight
				);
				var offsetHeight = Math.max(
					document.documentElement.offsetHeight,
					document.body.offsetHeight
				);
				
				if (scrollHeight < offsetHeight) {
					return $(window).height();
				} else {
					return scrollHeight;
				}
			} else {
				return $(document).height();
			}
		}
		
		this.bwidth = function() {
			if ($.browser.msie && $.browser.version < 7) {
				var scrollWidth = Math.max(
					document.documentElement.scrollWidth,
					document.body.scrollWidth
				);
				var offsetWidth = Math.max(
					document.documentElement.offsetWidth,
					document.body.offsetWidth
				);
				
				if (scrollWidth < offsetWidth) {
					return $(window).width();
				} else {
					return scrollWidth;
				}
			} else {
				return $(document).width();
			}
		}
		//Set the pop to display in the middle
		this.setCenterPosition = function() {
			var windowHeight = ((document.documentElement.clientHeight)/2 - (self.dh.height()/2));
			var windowWidth = ((document.documentElement.clientWidth)/2 - (self.dh.width()/2));
			
			//Yuki:get the position of current scrollbar
			var ScrollTop = document.body.scrollTop;
			if (ScrollTop == 0)	
			{
				if (window.pageYOffset)
					ScrollTop = window.pageYOffset;
				else
					ScrollTop = (document.body.parentElement) ? document.body.parentElement.scrollTop : 0;
			}
			var ScrollLeft = document.body.scrollLeft;
			if (ScrollLeft == 0)	
			{
				if (window.pageXOffset)
					ScrollLeft = window.pageXOffset;
				else
					ScrollLeft = (document.body.parentElement) ? document.body.parentElement.scrollLeft : 0;
			}
			var position_height = windowHeight + ScrollTop;
			var position_width = windowWidth+ScrollLeft;
			self.dh.css({position:'absolute',top: position_height, left: position_width});
			//self.dh.slideDown(1000); //slide open the pop_up window
		}
		
		this.setElementPosition = function() {
			var trigger = $(self.options.position.refele);
			var reftop = self.options.position.reftop || 0;
			var refleft = self.options.position.refleft || 0;
			var adjust = (typeof self.options.position.adjust=="undefined")?true:self.options.position.adjust;
			var top = trigger.offset().top + trigger.height();
			var left = trigger.offset().left;
			var docWidth = document.documentElement.clientWidth || document.body.clientWidth;
			var docHeight = document.documentElement.clientHeight|| document.body.clientHeight;
			var docTop = document.documentElement.scrollTop|| document.body.scrollTop;
			var docLeft = document.documentElement.scrollLeft|| document.body.scrollLeft;
			var docBottom = docTop + docHeight;
			var docRight = docLeft + docWidth;
			if (adjust && left + self.dh.width() > docRight) {
				left = docRight - self.dh.width() - 1;
			}
			if (adjust && top + self.dh.height() > docBottom) {
				top = docBottom - self.dh.height() - 1;
			}
			left = Math.max(left+refleft, 0);
			top = Math.max(top+reftop, 0);
			self.dh.css({top: top, left: left});
		}
		this.initOptions();
		this.initMask();
		this.initBox();		
		this.initContent();
		this.initEvent();
	}	
	
	var weeboxs = function() {		
		var self = this;
		this._onbox = false;
		this._opening = false;
		this.zIndex = 999;
		this.length = function() {
			return arrweebox.length;
		}
		this.open = function(content, options) {
			self._opening = true;
			if (typeof(options) == "undefined") {
				options = {};
			}
			if (options.boxid) {
				for(var i=0; i<arrweebox.length; i++) {
					if (arrweebox[i].dh.attr('id') == options.boxid) {
						arrweebox[i].close();
						break;
					}
				}
			}
			options.zIndex = self.zIndex;
			self.zIndex += 10;
			var box = new weebox(content, options);
			box.dh.click(function(){self._onbox = true;});
			arrweebox.push(box);
			return box;
		}
		this.getTopBox = function() {
			if (arrweebox.length>0) {
				return arrweebox[arrweebox.length-1];
			} else {
				return false;
			}
		}
		$(window).scroll(function() {
			if (arrweebox.length > 0) {
				for(i=0;i<arrweebox.length;i++) {
					var box = arrweebox[i];
					
					if (box.options.position != "center"){
						box.setElementPosition();
					}
					if (box.mh) {
						box.mh.css({
							width: box.bwidth(),
							height: box.bheight()
						});
					}
				}
			}		
		}).resize(function() {
			if (arrweebox.length > 0) {
				var box = self.getTopBox();
				if (box.options.position == "center") {
					box.setCenterPosition();
				}
				if (box.mh) {
					box.mh.css({
						width: box.bwidth(),
						height: box.bheight()
					});
				}
			}
		});
		$(document).click(function(event) {
			if (event.button==2) return true;
			if (arrweebox.length>0) {
				var box = self.getTopBox();
				if(!self._opening && !self._onbox && box.options.clickClose) {
					box.close();
				}
			}
			self._opening = false;
			self._onbox = false;
		});
	}
	$.extend({weeboxs: new weeboxs()});		
})(jQuery);

//Mask treatment
(function($){$.fn.bgIframe=$.fn.bgiframe=function(s){if($.browser.msie&&/6.0/.test(navigator.userAgent)){s=$.extend({top:'auto',left:'auto',width:'auto',height:'auto',opacity:true,src:'javascript:false;'},s||{});var prop=function(n){return n&&n.constructor==Number?n+'px':n;},html='<iframe class="bgiframe"frameborder="0"tabindex="-1"src="'+s.src+'"'+'style="display:block;position:absolute;z-index:-1;'+(s.opacity!==false?'filter:Alpha(Opacity=\'0\');':'')+'top:'+(s.top=='auto'?'expression(((parseInt(this.parentNode.currentStyle.borderTopWidth)||0)*-1)+\'px\')':prop(s.top))+';'+'left:'+(s.left=='auto'?'expression(((parseInt(this.parentNode.currentStyle.borderLeftWidth)||0)*-1)+\'px\')':prop(s.left))+';'+'width:'+(s.width=='auto'?'expression(this.parentNode.offsetWidth+\'px\')':prop(s.width))+';'+'height:'+(s.height=='auto'?'expression(this.parentNode.offsetHeight+\'px\')':prop(s.height))+';'+'"/>';return this.each(function(){if($('> iframe.bgiframe',this).length==0)this.insertBefore(document.createElement(html),this.firstChild);});}return this;};})(jQuery);

function pop_up(objvalue,page,funName,cancelfun) //page 0=one button && alert | 1= two button && confirm |
{
	if(page == "1")
		$.weeboxs.open(objvalue,{title:'Comcast Modem Configuration Help',width:433, height:218,onok:function(box){box.close();funName()},oncancel:function(box){box.close()},onopen:function(box){},onclose:function(box){}});
	else if(page == "0")
		$.weeboxs.open(objvalue,{title:'Comcast Modem Configuration Help',okBtnName:'Continue',width:433, height:218,onok:function(box){box.close();},type:'alert',showCancel:false});
	else if(page == "2")
		$.weeboxs.open(objvalue,{title:'Comcast Modem Configuration Help',okBtnName:'Continue',width:433, height:218,onok:function(box){box.close();funName()},type:'alert',showCancel:false});
	else if(page == "3")
		$.weeboxs.open(objvalue,{title:'Comcast Modem Configuration Help',width:433, height:218,onok:function(box){box.close();funName()},oncancel:function(box){box.close();cancelfun()},onopen:function(box){},onclose:function(box){}});
}
