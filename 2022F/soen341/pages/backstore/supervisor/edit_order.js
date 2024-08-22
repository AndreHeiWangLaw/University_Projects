let totalDom = document.getElementById('totalPrice');

function increaseAmount(id, unit_price) {
    dom = document.getElementById(id);
    amount = parseInt(dom.innerText) + 1;
    dom.innerText = amount;

    let price = (unit_price * amount).toFixed(2);

    let prodPriceDom = document.getElementById(id + "_total");

    prodPriceDom.innerText = '$' + price;

    totalDom.innerText = (parseFloat(totalDom.innerText) + unit_price).toFixed(2);
}

function decreaseAmount(id, unit_price) {
    if (parseInt(document.getElementById(id).innerText) == 1)
        return;

    dom = document.getElementById(id);
    amount = parseInt(dom.innerText) - 1;
    dom.innerText = amount;

    let price = (unit_price * amount).toFixed(2);

    let prodPriceDom = document.getElementById(id + "_total");

    prodPriceDom.innerText = '$' + price;

    totalDom.innerText = (parseFloat(totalDom.innerText) - unit_price).toFixed(2);
}

function removeFromTotal(price) {
    totalDom.innerText = (parseFloat(totalDom.innerText) - price).toFixed(2);
}

function addRow(id) {
    let prodIdDom = document.getElementById('newProductId');
    const data = new Map();

    let userId = document.getElementById("userId").value;

    doms = document.getElementsByClassName('order-row'); // Using empty class names to find rows
    for (let i = 0; i < doms.length; i++) {
        let dom = doms.item(i);
        let id = dom.getElementsByTagName('div').item(2).getElementsByTagName('input').item(0).value;
        let quantity = dom.getElementsByTagName('label').item(0).innerText;
        data.set(id, quantity);
    }
    data.set(prodIdDom.value, 1);

    const json = mapToObj(data);

    let form = document.createElement('form');
    form.method = "GET";
    form.action = "../order_handle_update.php";

    let idElement = document.createElement('input');
    idElement.name = "id";
    idElement.value = id;

    let userIdElement = document.createElement('input');
    userIdElement.name = "user_id";
    userIdElement.value = userId;

    let dataElement = document.createElement('input');
    dataElement.name = "data";
    dataElement.value = JSON.stringify(json);

    form.appendChild(idElement);
    form.appendChild(userIdElement);
    form.appendChild(dataElement);

    form.style.visibility = 'hidden';

    document.body.appendChild(form);

    form.submit();
}

function submitOrder(id) {
    const data = new Map();

    let userId = document.getElementById("userId").value;

    doms = document.getElementsByClassName('order-row'); // Using empty class names to find rows
    for (let i = 0; i < doms.length; i++) {
        let dom = doms.item(i);
        let id = dom.getElementsByTagName('div').item(2).getElementsByTagName('input').item(0).value;
        let quantity = dom.getElementsByTagName('label').item(0).innerText;
        data.set(id, quantity);
    }
    const json = mapToObj(data);

    let form = document.createElement('form');
    form.method = "GET";
    form.action = "../order_handle_update.php";

    let idElement = document.createElement('input');
    idElement.name = "id";
    idElement.value = id;

    let userIdElement = document.createElement('input');
    userIdElement.name = "user_id";
    userIdElement.value = userId;

    let dataElement = document.createElement('input');
    dataElement.name = "data";
    dataElement.value = JSON.stringify(json);

    form.appendChild(idElement);
    form.appendChild(userIdElement);
    form.appendChild(dataElement);

    form.style.visibility = 'hidden';

    document.body.appendChild(form);

    form.submit();
}


function deleteRow(id) {
    let prodPriceDom = document.getElementById(id + "_quantity_total");
    removeFromTotal(parseFloat(prodPriceDom.innerText.substring(1)));

    rowDom = document.getElementById(id);
    rowDom.remove();
}

function mapToObj(map) {
    const obj = {};
    for (let [k, v] of map)
        obj[k] = v;
    return obj;
}