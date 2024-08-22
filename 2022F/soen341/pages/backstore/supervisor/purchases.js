function deleteOrder(id) {
    fetch('../../../database/orders.json').then(response => {
        return response.json();
    }).then(orders => {
        var result = confirm("Are you sure to delete order #" + id + " from the list of orders?");
        if (result) {
            document.location = "../order_delete_handler.php?id=" + id;
        }
    }).catch(() => {
        console.log("Could not fetch data from products list")
    });
}

