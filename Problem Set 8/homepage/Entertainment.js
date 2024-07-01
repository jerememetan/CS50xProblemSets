document.addEventListener('DOMContentLoaded', function() {
    let crosshair = document.getElementById('crosshair');
    let main = document.querySelector('main');
    let gamescreen = document.getElementById('gameScreen');
    let header = document.querySelector('header');
    let game = document.getElementById('game');
    console.log(crosshair);
    console.log(typeof(crosshair));
    crosshair.addEventListener('click', function(){
        loadscreen();
        rungame();






    })
        function loadscreen()
        {
        gamescreen.style.margin = '5rem 0rem 5rem 0rem';
        gamescreen.style.transition = '2s';
        document.getElementById('boredtext').style.display = 'none';
        document.getElementById('boredtext').style.transition = '2s';
        game.style.backgroundColor = 'black';
        game.style.height = '38vw';
        game.style.width = '88vw';
        game.style.transition = '2s';
        game.style.position = 'relative';
        }

        function rungame() {
        let titlescreen = document.getElementById('titlescreen');
        let startButton = document.getElementById('startButton');
        titlescreen.style.border = '15rem';
        titlescreen.style.visibility = 'visible';
        titlescreen.style.opacity = '100';
        titlescreen.style.color = '#b0f8ff';
        startButton.style.visibility = 'visible';
        startButton.style.backgrondColor = '#b0f8ff';
        startButton.style.opacity = '100';
        startButton.style.transition = '2s';
        startButton.style.position = 'absolute';
        startButton.style.bottom = '15vw';
        startButton.style.left = '41vw';
        startButton.addEventListener('click', function(){
                titlescreen.style.color = '#f55885';
                titlescreen.style.transition = '2s';
                startButton.style.visibility = 'hidden';
                startButton.style.transition = '0s';
                let gametext = document.getElementById('gametext')
                gametext.style.visibility = 'visible';
                gametext.style.opacity = '100';
                gametext.style.color = '#f55885';
                gametext.style.transition = '3s';
                startButton.style.position = 'relative';


        })
        return 0;
    }
  })


