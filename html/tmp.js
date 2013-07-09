var shown;

function load(url, cb) // eventually allow a callback
{
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState < 4)
            return;
        if (xmlhttp.status != 200)
            return;
	if (xmlhttp.readyState == 4)
	{
	    if (cb)
	    {
		    cb(xmlhttp.responseText);
	    }
            return; // This wil probably be a callback later.
	}
    };
    xmlhttp.open('GET',url, true);
    xmlhttp.send('');
}

function removeLinks(which)
{
	if (which.tagName == 'SCRIPT' || which.tagName == 'LINK')
	{
		which.parentNode.removeChild(which);
		return;
	}
	var t = which.firstChild;
	while (t)
	{
		var g = t;
		t = g.nextSibling;
		removeLinks(g);
	}
}

function showme(which) 
{
    if (document.getElementById(which)) 
    {
        if (shown) 
        {
            shown.style.display = 'none';
            if (shown.id == which) 
            {
                shown = null;
                return 
            }
        }
        shown = document.getElementById(which);
        if (! shown.already_displayed)
        {
            var localshown = shown;
            load('/?content=' + encodeURIComponent(shown.getAttribute('data-guid')), function (txt) { if (localshown.already_displayed) return; localshown.already_displayed = 1; var d = document.createElement('div'); d.innerHTML = txt; removeLinks(d); localshown.appendChild(d); localshown.parentNode.scrollIntoView(1); } );
        }
        shown.style.display = 'block'; 
        shown.parentNode.scrollIntoView(1);
        var bar = shown.parentNode.firstChild;
        while (bar && !( bar.id == 'item')) 
        {
            bar = bar.nextSibling;
        }
        if (bar) 
        {
            bar.style.backgroundColor='#EEEEEE';
        }
    }
}

function mj() 
{
    if (shown) 
    {
        showme('spec' + String(parseInt(shown.id.toString().substr(4))+1)) 
    }
}

function mk() 
{
    if (shown) 
    {
        showme('spec' + String(parseInt(shown.id.toString().substr(4))-1)) 
    }
}

document.onkeypress = function(e) 
{
    if (e.charCode == 74 || e.charCode == 106) 
    {
        mj();
    }
    else if (e.charCode == 75 || e.charCode == 107) 
    {
        mk();
    }
};


function handlekeypress(e)
{
    if (e.target.id == "addurl")
    {
	if (e.keyCode == 13) // Enter
	{
		load('/?addurl=' + e.target.value, function(txt) {
			var newnode = document.createElement('div');
			newnode.innerHTML = txt;
			newnode = newnode.firstChild;
			e.target.parentNode.innerHTML = newnode.innerHTML;
		});
	}
    }
    return 0;
}

window.onload = function() {
	document.addEventListener("keypress", handlekeypress);
}
