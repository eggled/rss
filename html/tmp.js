var shown;

// accept text, return text.
function removeLinks(which)
{
	var ret = $("<div></div>");
	ret.html(which);
	ret.find('script').remove();
	ret.find('link').remove();
	return ret.html();
}

function showme(which) 
{
//alert("content_" + which);
//alert($("content_" +which).length);
//$(which).load('?content=' + encodeURIComponent($(which).attr('data-guid')));
window.location.href = '#' + which;
return;
}

function loadnew(i)
{
	//$('spec' + i).load('?content=' + encodeURIComponent($('spec' + i).attr('data-guid')));
	if ($('#spec' + i).length > 0)
		window.location.replace('#spec' + i);
}
	

function mj() 
{
	var n = window.location.href.toString().split('#');
	if (n.length > 1) {
		if (n[1].substring(0,4) == "spec") {
			loadnew(String(parseInt(n[1].substring(4))+1));
		}
	}
}

function mk() 
{
	var n = window.location.href.toString().split('#');
	if (n.length > 1) {
		if (n[1].substring(0,4) == "spec") {
			loadnew(String(parseInt(n[1].substring(4))-1));
		}
	}
}

function handlekeypress(e)
{
    if (e.target.id == "addurl")
    {
	if (e.keyCode == 13) // Enter
	{
		$.ajax('/?addurl=' + e.target.value).done(function(html) 
				{
				var newnode = document.createElement('div');
				newnode.innerHTML = html;
				newnode = newnode.firstChild;
				html = newnode.innerHTML;
				$("#addurl").parent().html(html)
				});
	}
    } else {
	    if (e.charCode == 74 || e.charCode == 106) 
	    {
		    mj();
	    }
	    else if (e.charCode == 75 || e.charCode == 107) 
	    {
		    mk();
	    }
    }
    return 0;
}

window.onload = function() {
	$(document).on('keypress',handlekeypress);
	$(document).on('swipeleft', mj);
	$(document).on('swiperight', mk);
	$("div").on("pageshow", 
			function(e, ui) {
			var which = e.target;
			var url = "/?content=" + encodeURIComponent($("#content_" + which.id).attr("data-guid"));
			$.get(url,
				function(html) { 
				$("#content_" + which.id).html(removeLinks(html)); 
				$("#link_" + which.id).css("background-color","#ccc");
				}); 
			});
	//$("div").on("pageshow", function(e, ui) { $("#content_" + e.target.id).load("/?content=" + encodeURIComponent($("#content_" + e.target.id).attr('data-guid'))); });
}
//$(document).bind("mobileinit",function() {
		//$.mobile.defaultPageTransition = "slide";
		//});
