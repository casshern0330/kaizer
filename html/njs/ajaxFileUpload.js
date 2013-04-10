var ajaxFileUpload = function(opts){
    return new ajaxFileUpload.prototype.init(opts);
};
ajaxFileUpload.prototype = {
    init:function(opts){
        var set = this.extend({
            url:'result.php',
            id:'fileId',
            callback:function(){}
        },opts || {});
        var _this = this;
        var id = +new Date();
        var form = this.createForm(id),frame = this.createIframe(id,set.url);
        var oldFile = document.getElementById(set.id)
        var newFile = oldFile.cloneNode(true);
        var fileId = 'ajaxFileUploadFile'+id;
        oldFile.setAttribute('id',fileId);
        oldFile.parentNode.insertBefore(newFile,oldFile);
        form.appendChild(oldFile);//ע���������ȫ���⣬Ҫ��ԭ�ļ���ŵ�������form���ύ
        form.setAttribute('target',frame.id);//��form��target����Ϊiframe,�����ύ�󷵻ص����ݾ���iframe��
        form.setAttribute('action',set.url);
        setTimeout(function(){
            form.submit();
            if(frame.attachEvent){
                frame.attachEvent('onload',function(){_this.uploadCallback(id,set.callback);});
            }else{
                frame.onload = function(){_this.uploadCallback(id,set.callback);}
            }
        },100);
    },
    /*
        ����iframe��ie7��6�Ƚϵ��ۣ��������������������������ת
    */
    createIframe:function(id,url){
        var frameId = 'ajaxFileUploadFrame'+id,iFrame;
        var IE = /msie ((\d+\.)+\d+)/i.test(navigator.userAgent) ? (document.documentMode ||  RegExp['\x241']) : false,
        url = url || 'javascript:false';
        if(IE && IE < 8){
            iFrame = document.createElement('<iframe id="' + frameId + '" name="' + frameId + '" />');
            iFrame.src = url;
        }else{
            iFrame = document.createElement('iframe');
            this.attr(iFrame,{
                'id':frameId,
                'name':frameId,
                'src':url
            });
        };
        iFrame.style.cssText = 'position:absolute; top:-9999px; left:-9999px';
        return document.body.appendChild(iFrame);
    },
    /*
        ����form
    */
    createForm:function(id){
        var formId = 'ajaxFileUploadForm'+id;
        var form = document.createElement('form');
        this.attr(form,{
            'action':'',
            'method':'POST',
            'name':formId,
            'id':formId,
            'enctype':'multipart/form-data',
            'encoding':'multipart/form-data'
        });
        form.style.cssText = 'position:absolute; top:-9999px; left:-9999px';
        return document.body.appendChild(form);  
    },
    /*
        ��ȡiframe���ݣ�ִ�лص����������Ƴ����ɵ�iframe��form
    */
    uploadCallback:function(id,callback){
        var frame = document.getElementById('ajaxFileUploadFrame'+id),form = document.getElementById('ajaxFileUploadForm'+id);data = {};
        var db = document.body;
        try{
            if(frame.contentWindow){
                data.responseText = frame.contentWindow.document.body ? frame.contentWindow.document.body.innerHTML : null;
                data.responseXML = frame.contentWindow.document.XMLDocument ? frame.contentWindow.document.XMLDocument : frame.contentWindow.document;
            }else{
                data.responseText = frame.contentDocument.document.body ? frame.contentDocument.document.body.innerHTML : null;
                data.responseXML = frame.contentDocument.document.XMLDocument ? frame.contentDocument.document.XMLDocument : frame.contentDocument.document;
            }
        }catch(e){};
        callback && callback.call(data);
        setTimeout(function(){
            db.removeChild(frame);
            db.removeChild(form);
        },100);
    },
    attr:function(el,attrs){
        for(var prop in attrs) el.setAttribute(prop,attrs[prop]);
        return el;
    },
    extend:function(target,source){
        for(var prop in source) target[prop] = source[prop];
        return target;
    }
};
ajaxFileUpload.prototype.init.prototype = ajaxFileUpload.prototype;