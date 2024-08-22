-- Create the Member table
CREATE TABLE Member (
    memberId INT PRIMARY KEY,
    firstName VARCHAR(255),
    lastName VARCHAR(255),
    middleName VARCHAR(255),
    dateOfBirth DATE,
    address VARCHAR(255),
    gender VARCHAR(255),
    phoneNumber VARCHAR(255),
    email VARCHAR(255),
    socialInsuranceNumber VARCHAR(255),
    startDateMembership DATE,
    memberType ENUM('client', 'employee', 'donor')
);

-- Create the Donation table
CREATE TABLE Donation (
    donationId INT PRIMARY KEY,
    donorId INT,
    date DATE,
    type ENUM('money', 'product'),
    amount INT,
    memberId INT,
    FOREIGN KEY (memberId) REFERENCES Member(memberId)
);

-- Create the Product table
CREATE TABLE Product (
    productId INT PRIMARY KEY,
    description VARCHAR(255),
    date DATE,
    sellingPrice INT,
    weight FLOAT,
    inStock BOOLEAN
);

-- Create the Sales table
CREATE TABLE Sales (
    salesId INT PRIMARY KEY,
    date DATE,
    amount INT,
    memberId INT,
    FOREIGN KEY (memberId) REFERENCES Member(memberId)
);

-- Create the SalesItems table
CREATE TABLE SalesItems (
    salesItemId INT PRIMARY KEY,
    productId INT,
    salesId INT,
    FOREIGN KEY (productId) REFERENCES Product(productId),
    FOREIGN KEY (salesId) REFERENCES Sales(salesId)
);

-- Create the Expenses table
CREATE TABLE Expenses (
    expenseId INT PRIMARY KEY,
    presidentId INT,
    date DATE,
    amount INT,
    type ENUM('rent', 'bill', 'charity'),
    description VARCHAR(255),
    memberId INT,
    FOREIGN KEY (presidentId) REFERENCES Member(memberId),
    FOREIGN KEY (memberId) REFERENCES Member(memberId)
);

-- Create the Employee table
CREATE TABLE Employee (
    employeeId INT PRIMARY KEY,
    memberId INT,
    jobTitle ENUM('president', 'vice-president', 'cashier', 'other'),
    salary INT,
    FOREIGN KEY (memberId) REFERENCES Member(memberId)
);
