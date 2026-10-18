const texto = document.querySelector('.texto');

function clicktest(){
    texto.addEventListener('click', () => {
        texto.style.color = 'red';
    });
}

clicktest();