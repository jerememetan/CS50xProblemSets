document.addEventListener('DOMContentLoaded', function() {
    function getAge(){
        var currenttime = new Date();
        var y = currenttime.getFullYear();
        var age = y - 2002;
        document.getElementById("AGE").innerHTML = age;}
    getAge();




})



