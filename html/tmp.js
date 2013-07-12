var shown;

function removeLinks(which)
{
	which.find('script').remove();
	which.find('link').remove();
}

function showme(which) 
{
    if ($('#' + which))
    {
        if (shown) 
        {
            shown.css('display', 'none');
            if (shown.attr('id') == which) 
            {
                shown = null;
                return 
            }
        }
	if ($('#' + which).length == 0)
		return;
        shown = $('#' + which);
        if (shown)
        {
            var localshown = shown;
	    $.ajax('/?content=' + encodeURIComponent(shown.attr('data-guid')),
		{ success : function (txt,stat,other) {
			var d = $('<div></div>');
			d.html(txt);
			d.attr('id','contentdiv');
			removeLinks(d); 
			if(localshown.find('#contentdiv').length)
			{
				localshown.find('#contentdiv').replaceWith(d);
			} else {
				localshown.append(d);
			}
		}});
        }
        shown.css('display', 'block');
        //shown.parentNode.scrollIntoView(1);
        var bar = shown.parent().find('#item');
        if (bar) 
        {
            bar.css('background-color', '#EEEEEE');
        }
    }
}

function mj() 
{
    if (shown) 
    {
        showme('spec' + String(parseInt(shown.attr('id').toString().substr(4))+1)) 
    }
}

function mk() 
{
    if (shown) 
    {
        showme('spec' + String(parseInt(shown.attr('id').toString().substr(4))-1)) 
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
}
