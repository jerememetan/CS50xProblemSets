document.addEventListener('DOMContentLoaded', function() {
    let localStorage = window.sessionStorage;
    console.log('isLight?', localStorage.getItem('isLightMode'));
    let toggle = document.getElementById('modetoggle');
    let body = document.querySelector('body');
    let nav = document.querySelector('nav');
    let navlink = nav.querySelectorAll('.nav-link');
    let table = document.querySelectorAll(".table");
    let input = document.querySelectorAll(".custom-input");
    let modal = document.querySelector(".modal-content");

    toggle.addEventListener('click', function() {
        // dark mode
        this.classList.toggle('bi-moon-fill');
        if (this.classList.contains('bi-moon-fill')) {
            localStorage.setItem('isLightMode', '0');
            body.style.transition = '1s';
            nav.style.transition = '1s';
            toggle.style.transition = '1s';
            darkmode()
            if (table) {
                for (let i = 0; i < table.length; i++) {
                    let thead = table[i].querySelectorAll("th");
                    let tdata = table[i].querySelectorAll('td');
                    for (let head of thead) {
                        head.style.transition = '1s';
                    }
                    for (let data of tdata) {
                        data.style.transition = '1s';
                    }
                } }

                if (input){
                    for (let i = 0; i < input.length; i++) {
                        input[i].style.transition = '1s';
                    }
                }

                for (let nav of navlink) {
                    if (nav.id != "del") {
                        nav.style.transition = "1s";
                    }
                }





        } else {
            localStorage.setItem('isLightMode', '1');
            body.style.transition = '1s';
            nav.style.transition = '1s';
            toggle.style.transition = '1s';
            lightmode()
            if (table) {
                for (let i = 0; i < table.length; i++)
                     {
                    let thead = table[i].querySelectorAll("th");
                    let tdata = table[i].querySelectorAll('td');
                    for (let head of thead)
                        {
                            head.style.transition = '1s';
                        }
                    for (let data of tdata)
                        {
                            data.style.transition = '1s'
                        }
                    }

                    }
            if (input){
                for (let i = 0; i < input.length; i++)
                    {
                        input[i].style.transition = '1s';
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
        body.style.backgroundColor = '#141e2e';
        body.style.color = '#bed0e6';
        nav.style.background = '#11225c';
        nav.style.color = '#6f7782';
        toggle.style.color = '#dce080';

        if (table) {
            for (let i = 0; i < table.length; i++) {
                let thead = table[i].querySelectorAll("th");
                let tdata = table[i].querySelectorAll('td');
                for (let head of thead) {
                    head.style.backgroundColor = '#151d26';
                    head.style.color = "white";
                }
                for (let data of tdata) {
                    data.style.backgroundColor = '#2d3642';
                    data.style.color = "white";
                }
            }
        }
        if (input){
            for (let i = 0; i < input.length; i++) {
                input[i].style.backgroundColor = '#101317';
                input[i].style.color = '#bed0e6';
                if (input[i].placeholder && input[i].placeholder.style){
                    input[i].placeholder.style.color = 'white';
                }
            }
        }

        if (modal){
            modal.style.backgroundImage = 'linear-gradient(to top, #1f1336, #131836)'
        }
        for (let nav of navlink) {
            if (nav.id != "del") {
                nav.style.color = "#bed0e6";
            }
        }

    }

    function lightmode() {
        body.style.backgroundColor = '#dae0eb';
        body.style.color = '#242323';
        nav.style.background = '#a3aed4';
        nav.style.color = 'black';
        toggle.style.color = '#5a61a1';

        if (table) {
            for (let i = 0; i < table.length; i++) {
                let thead = table[i].querySelectorAll("th");
            let tdata = table[i].querySelectorAll('td');
            for (let head of thead) {
                head.style.backgroundColor = 'white';
                head.style.color = "black";
                }
            for (let data of tdata) {
                data.style.backgroundColor = '#c1d7f5';
                data.style.color = "black";
                }
            }

        }
        if (input){
            for (let i = 0; i < input.length; i++) {
                input[i].style.backgroundColor = 'white';
                input[i].style.color = '#242323';
                if (input[i].placeholder && input[i].placeholder.style){
                    input[i].placeholder.style.color = 'red';
                }
            }
        }
        if (modal){
            modal.style.backgroundImage = 'linear-gradient(to top, #f0f3f7, #9ba0bf)'
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
