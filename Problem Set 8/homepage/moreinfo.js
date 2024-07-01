document.addEventListener('DOMContentLoaded', function() {

})

let arrayEng =['5qYMFV8EdILwgFbCwyG85Y','4E6aGvjFv9vQfe6MOml2KQ','30sX5AnSQlk25MO81QSAxR','6FLZ9tqFBcxK5TF9LDS4H3', '3sSFoPNflZZQswI9jscK08'];
let arrayStu = ['37i9dQZF1DWSSrwtip3vZP','37i9dQZF1DWZeKCadgRdKQ','2ndjIXc08a5al6z82AT9Kd','7oo5FIeqIbIoUaeQ66xZFu','0vvXsWCC9xrXsKd4FyS8kM','01NxZ4RDheediA6ICD2ii7'];
let arrayJap =['2mEvhXLjPNh3enzxGv5utC','6MCML3pfj3ADBgJ3RSTtWi','5G4tuOGiXl3YAmK8kUEEGR','433seUV4wWQIXu0wVSqjN7', '7gyeEhrwgxezLxlqhuJa1b'];
let arrayChi =['37i9dQZF1EIfCThMnXp12G','1QnXJhL6xyymR2Epze9qAf','0uaC4JoRcb0ucbozjfFbLq','37i9dQZF1EIfCThMnXp12G','5hFNF2SpC7jWud233ZFHYm'];
let arrayAll = [...arrayEng, ...arrayStu, ...arrayJap, ...arrayChi];



   function randomsiteAll(){
        let randindex = Math.random() * arrayAll.length;
        randindex = parseInt(randindex, 10);
        location.href = 'https://open.spotify.com/playlist/' + arrayAll[randindex];
        location.target="_blank"
    }

    function randomsiteEnglish(){
        let randindex = Math.random() * arrayEng.length;
        randindex = parseInt(randindex, 10);
        location.href = 'https://open.spotify.com/playlist/' + arrayEng[randindex];
    }

    function randomsiteStudy(){
        let randindex = Math.random() * arrayStu.length;
        randindex = parseInt(randindex, 10);
        location.href = 'https://open.spotify.com/playlist/' + arrayStu[randindex];
    }

    function randomsiteJapanese(){
        let randindex = Math.random() * arrayJap.length;
        randindex = parseInt(randindex, 10);
        location.href = 'https://open.spotify.com/playlist/' + arrayJap[randindex];
    }

    function randomsiteChinese(){
        let randindex = Math.random() * arrayChi.length;
        randindex = parseInt(randindex, 10);
        location.href = 'https://open.spotify.com/playlist/' + arrayChi[randindex];
    }
