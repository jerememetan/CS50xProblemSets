function checkpassword(){
    let password = document.getElementsByName('password')[0].value;
    let cfmpassword = document.getElementsByName('cfmpassword')[0].value;
    if (password != cfmpassword && cfmpassword.length > 0 && password.length > 0)
    {
        document.getElementById('error').innerHTML =  '<small><i class="bi bi-exclamation-octagon-fill"></i> Password does not match</small>';
    }
    else
    {
        document.getElementById('error').innerHTML = '';
    }
    if (password.length > 0 && password.length < 8)
    {
        document.getElementById('infotext').innerHTML = '<small style="color: red"><i class="bi bi-info-circle-fill"></i> Password is not at least 8 characters</small>'
    }
    else if (password.length >= 8)
    {
        document.getElementById('infotext').innerHTML = '<small style="color: #17bd43"><i class="bi bi-check2-circle"></i> Password meets requirements!</small>'
    }
    else
    {
        document.getElementById('infotext').innerHTML = '<small><i class="bi bi-exclamation-octagon"></i> Password must be at least 8 characters</small>'
    }
}
