
PART I

=====================================================================
a)

CREATE TABLE donors(
    donorID INT(100) NOT NULL AUTO_INCREMENT,
    firstName VARCHAR(100) NOT NULL,
    lastName VARCHAR(100) NOT NULL,
    middleInitial VARCHAR(1) NOT NULL,
    dateOfBirth DATE,
    address VARCHAR(100) NOT NULL,
    city VARCHAR(100) NOT NULL,
    postalCode VARCHAR(100) NOT NULL,
	province VARCHAR(100)  NOT NULL,
    gender VARCHAR(100) NOT NULL,
    SSN VARCHAR(100) NOT NULL,
    PRIMARY KEY(donorID)
);

CREATE TABLE donations (
    dID INT(100) NOT NULL AUTO_INCREMENT,
    donorID INT(100),
    date DATE,
    type VARCHAR(100),
    amount DECIMAL(20, 2),
    PRIMARY KEY(dID),
    FOREIGN KEY (donorID) REFERENCES donors(donorID)
);

CREATE TABLE products (
    pID INT(100) NOT NULL AUTO_INCREMENT,
    description VARCHAR(100),
    date DATE,
    price DECIMAL(20, 2),
    weight DECIMAL(20, 2),
    isInStock BOOLEAN,
    PRIMARY KEY(pID)
);

CREATE TABLE sales (
    sID INT(100) NOT NULL AUTO_INCREMENT,
    date DATE,
    amount DECIMAL(20, 2),
    deliveryFee DECIMAL(20, 2),
    PRIMARY KEY(sID)
);

CREATE TABLE salesItems (
  sID INT(100) NOT NULL AUTO_INCREMENT,
  pID INT(100) NOT NULL,
  FOREIGN KEY (sID) REFERENCES sales(sID),
  FOREIGN KEY (pID) REFERENCES products(pID),
  PRIMARY KEY (sID, pID)
);


=====================================================================
b)

ALTER TABLE donors
DROP COLUMN middleInitial;


=====================================================================
c)

ALTER TABLE donors
ADD phone VARCHAR(100) DEFAULT 'unknown',
ADD email VARCHAR(100) DEFAULT 'unknown';


=====================================================================
d)

INSERT INTO products (pID, description, date, price, weight, isInStock)
VALUES (1, 'product_1', '2023-07-13', 1.00, 100.00, false);

INSERT INTO products (pID, description, date, price, weight, isInStock)
VALUES (2, 'product_2', '2023-07-14', 10.00, 10.00, true);

INSERT INTO products (pID, description, date, price, weight, isInStock)
VALUES (3, 'product_3', '2023-07-15', 100.00, 1.00, true);


=====================================================================
e)

DELETE FROM products;


=====================================================================
f)

DROP TABLE salesItems;
DROP TABLE sales;
DROP TABLE products;
DROP TABLE donations;
DROP TABLE donors;


=====================================================================
=====================================================================
=====================================================================

PART II
=====================================================================
a)

SELECT donorID, firstName, lastName, dateOfBirth, phone, email, SSN
FROM donors
WHERE gender = 'Female' AND province = 'Québec';


=====================================================================
b)

SELECT salesItem.sID, salesItem.pID, product.description, product.price, product.weight
FROM salesItems salesItem 
JOIN sales sale ON sale.sID = salesItem.sID
JOIN products product ON product.pID = salesItem.pID
WHERE sale.date = '2023-07-01' AND sale.deliveryFee > 0;


=====================================================================
c)

SELECT SUM(deliveryFee) AS allDeliveryFees
FROM sales
WHERE date >= '2023-06-01' AND date <= '2023-06-30';


=====================================================================
d)

SELECT MONTH(date) AS month, COUNT(*) AS allSales, SUM(amount) AS allAmount, SUM(deliveryFee) AS allDeliveryFees
FROM sales
WHERE date >= '2022-01-01' AND date <= '2023-12-31'
GROUP BY MONTH(date);


=====================================================================
e)

SELECT donor.donorID, donor.gender, donor.firstName, donor.lastName, SUM(donation.amount) AS allDonations
FROM donors donor
JOIN donations donation ON donation.donorID = donor.donorID
WHERE donor.city = 'Montréal' AND donation.date >= '2022-01-01' AND donation.date <= '2023-12-31'
GROUP BY donor.donorID, donor.firstName, donor.lastName
ORDER BY donor.gender ASC, donor.lastName ASC, donor.firstName ASC;



