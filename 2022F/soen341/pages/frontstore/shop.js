function editProduct(product_id) {
    //document.location = 'inventory_edit.php?id=' + product_id;
}

fetch('../../../database/inventory.json').then(response => {
    return response.json();
}).then(products => {
    displayProducts(products);
}).catch(err => {
    console.log("Could not fetch data from products list")
});

const productsContainer = document.getElementById("productsContainer");
let form = document.getElementById("form");

const urlParams = new URLSearchParams(window.location.search);
const username = urlParams.get('username');

if (username != null) {
    //console.log(username);
}

function displayProducts(products){

    for (const product in products){
        for (const copy in products[product]['copies']){

            //fix this, remove for (const copy in products[product]['copies']){ but we want displayed the total quantity of books and the cheapest price

                var shortenedProductName

                if (products[product]['product_name'].length > 14){
                    shortenedProductName = products[product]['product_name'].substring(0,11) + '...'
                } else {
                    shortenedProductName = products[product]['product_name']
                }

                const div = document.createElement('div');
                div.className = "col-6 col-md-6 col-lg-3";
                div.id = products[product]['product_name'];
                div.innerHTML = "<div class=\"card\">\n" +
                    "<div class=\"ImgContainer\">\n" +
                    "<img src=\"../../../" + products[product]['imageLink'] + "\" alt=\"" + products[product]['product_name'] + "\" class=\"card-img-top\" style=\"width: 100%\">\n" +
                    "<div class=\"top-right\">\n" +
                    "<a role=\"button\">\n" +
                    "<button type=\"button\" class=\"btn\" onclick='editProduct(\"" + products[product]['product_id'] + "\")'>\n" +
                    "<i class=\"fa fa-edit\" style=\"font-size:24px\"></i>\n" +
                    "</button>\n" +
                    "</a>\n" +
                    "</div>\n" +
                    "</div>\n" +
                    "<div class=\"card-body\">\n" +
                    "<div class=\"row transparent\" style=\"padding: 0px; border: 0px;\">\n" +
                    "<div class=\"col-sm d-none d-sm-none d-md-block\" style=\"padding: 0px; border: 0px;\">\n" +
                    "<p class=\"card-text\">Name</p>\n" +
                    "<p class=\"card-text\">Price</p>\n" +
                    "<p class=\"card-text\">Quantity</p>\n" +
                    "<p class=\"card-text\"><b>Supplier</b></p>\n" +
                    "</div>\n" +
                    "<div class=\"col-sm\" style=\"padding: 0px; border: 0px;\">\n" +
                    "<p class=\"card-text\">" + shortenedProductName + "</p>\n" +
                    "<p class=\"card-text\">$" + products[product]['copies'][copy]['price'] + "</p>\n" +
                    "<p class=\"card-text\">" + products[product]['copies'][copy]['quantity'] + "</p>\n" +
                    "<p class=\"card-text\"><b>" + products[product]['copies'][copy]['supplier'] + "</b></p>\n" +
                    "</div>\n" +
                    "</div>\n" +
                    "</div>\n" +
                    "<a class=\"btn btn-dark btn-lg\" role=\"button\" id='" + products[product]['product_name'] + "Button'><h3>Buy</h3></a>\n" +
                    "</div>\n";

                productsContainer.insertBefore(div,document.getElementById("emptyContainer"));

                var productButton = document.getElementById(products[product]['product_name'] + "Button");

                productButton.addEventListener("click", ()=>{
                    var productName = products[product]['product_name'];
                    console.log(productName + " has been clicked");
                    //fill the invisible form to send to PHP server to make a request in json list
                    var result = confirm(productName + " cannot be bought at the moment ");
                    // if (result){
                    //     document.getElementById("input_product_name").value = productName;
                    //     form.submit();
                    // }
                })

        }

    }
}
