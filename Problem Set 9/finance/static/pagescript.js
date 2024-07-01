document.addEventListener('DOMContentLoaded', function() {
    let localStorage = window.sessionStorage;
    console.log('isLight?', localStorage.getItem('isLightMode'));
    let toggle = document.getElementById('modetoggle');
    let body = document.querySelector('body');
    let nav = document.querySelector('nav');
    let navlink = nav.querySelectorAll('.nav-link');
    let table = document.querySelector(".table");
    toggle.addEventListener('click', function() {
        // dark mode
        this.classList.toggle('bi-moon-fill');
        if (this.classList.contains('bi-moon-fill')) {
            localStorage.setItem('isLightMode', '0');
            darkmode()
            body.style.transition = '1s';
            nav.style.transition = '1s';
            toggle.style.transition = '1s';
            if (table) {
                let thead = table.querySelectorAll("th");
                let tdata = table.querySelectorAll('td');
                for (let head of thead) {
                    head.style.transition = '1s';
                }
                for (let data of tdata) {
                    data.style.transition = '1s';
                }
            }
            for (let nav of navlink) {
                if (nav.id != "del") {
                    nav.style.transition = "1s";
                }
            }


        } else {
            localStorage.setItem('isLightMode', '1');
            lightmode()
            body.style.transition = '1s';
            nav.style.transition = '1s';
            toggle.style.transition = '1s';
            if (table) {
                let thead = table.querySelectorAll("th");
                let tdata = table.querySelectorAll('td');
                for (let head of thead) {
                    head.style.transition = '1s';
                }
                for (let data of tdata) {
                    data.style.transition = '1s'
                }
            }
            for (let nav of navlink) {
                if (nav.id != "del") {
                    nav.style.transition = "1s";
                }

            }

        }
    })


    function darkmode() {
        body.style.background = '#212121';
        body.style.color = '#bed0e6';
        nav.style.background = '#55595e';
        nav.style.color = '#6f7782';
        toggle.style.color = '#dce080';

        if (table) {
            let thead = table.querySelectorAll("th");
            let tdata = table.querySelectorAll('td');
            for (let head of thead) {
                head.style.backgroundColor = '#151d26';
                head.style.color = "white";
            }
            for (let data of tdata) {
                data.style.backgroundColor = '#2d3642';
                data.style.color = "white";
            }
        }
        for (let nav of navlink) {
            if (nav.id != "del") {
                nav.style.color = "#bed0e6";
            }
        }

    }

    function lightmode() {
        body.style.backgroundColor = '#e6e3e3';
        body.style.color = '#242323';
        nav.style.background = '#d5dbe3';
        nav.style.color = 'black';
        toggle.style.color = '#5a61a1';

        if (table) {
            let thead = table.querySelectorAll("th");
            let tdata = table.querySelectorAll('td');
            for (let head of thead) {
                head.style.backgroundColor = 'white';
                head.style.color = "black";

            }
            for (let data of tdata) {
                data.style.backgroundColor = '#c1d7f5';
                data.style.color = "black";
            }

        }
        for (let nav of navlink) {
            if (nav.id != "del") {
                nav.style.color = "black";
            }

        }
    }

    if (localStorage.getItem('isLightMode') == 1) {

        lightmode();
    } else {
        toggle.classList.toggle('bi-moon-fill')
        darkmode();
    }
})
