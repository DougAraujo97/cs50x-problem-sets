document.addEventListener('DOMContentLoaded', function() {
    document.querySelector('#name-button').addEventListener('click', function() {
        if (document.querySelector('#user-name').value != "") {
            let name = document.querySelector('#user-name').value;
        alert('Hello, ' + name + '!')
        }
    });
});