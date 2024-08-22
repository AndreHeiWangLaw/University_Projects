var form = document.getElementById('form');
var submit = document.getElementById('submitButton');
var email, password;

submit.addEventListener("click", () => {
    console.log("test")
    email = document.getElementById('email')
    password = document.getElementById('password')

    if (email.value == '')
        alert("Please specify an email address")
    else if (password.value == '')
        alert("Please enter your password")
    else
        form.submit();
});