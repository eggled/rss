var shown;
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
        var data = shown.innerHTML.toString();
        data = data.replace('<!--<<<<<', '');
        data = data.replace('>>>>>-->', '');
        shown.innerHTML = data;
        shown.style.display = 'block';
        shown.parentNode.scrollIntoView(1);
        var bar = shown.parentNode.firstChild;
        while (bar && !( bar.id == 'item')) 
        {
            bar = bar.nextSibling;
        }
        if (bar) 
        {
            //FIXME this is now read ... use data-guid to record in the database.
            //alert('marking ' + shown.getAttribute('data-guid') + ' as read.');
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
