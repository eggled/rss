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
	var localshown = shown;
	load('GET /?content=' + encodeURIComponent(shown.getAttribute('data-guid')), function (txt) { localshown.innerHTML = txt; localshown.style.display = 'block'; localshown.parentNode.scrollIntoView(1);} );

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
