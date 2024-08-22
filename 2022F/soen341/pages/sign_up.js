let form = document.getElementById('form');
const submit = document.getElementById('submitButton');
let firstName,lastName,email,address,postalCode,password,confirm_password;

const urlParams = new URLSearchParams(window.location.search);
const message = urlParams.get('msg');

if ( message != null ){
    const input = document.createElement("h6");
    input.innerHTML = message;
    document.body.insertBefore(input,document.getElementById("form"));
}

submit.addEventListener("click", () => {
    firstName = document.getElementById('firstName')
    lastName = document.getElementById('lastName')
    email = document.getElementById('email')
    address = document.getElementById('address')
    postalCode = document.getElementById('postalCode')
    password = document.getElementById('password')
    confirm_password = document.getElementById('confirm_password')
    role_customer = document.getElementById('role_customer')
    role_supplier = document.getElementById('role_supplier')
    role_supervisor = document.getElementById('role_supervisor')


    if (firstName.value == '') {
        alert("Please add a first name")
    } else if (lastName.value == '') {
        alert("Please add a last name")
    } else if (email.value == '') {
        alert("Please add an email address")
    } else if (address.value == ''){
        alert("Please add an address")
    } else if (postalCode.value == ''){
        alert("Please add a postal code")
    } else if (password.value == ''){
        alert("Please add a password")
    } else if (confirm_password.value !== password.value) {
        alert("Please add a password that matches")
    } else if (role_customer.value == '' && role_supplier.value == '' && role_supervisor.value == ''){
        alert("Please select a role")
    } else {
        form.submit();
    }

});