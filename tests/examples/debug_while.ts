function testSimpleWhile(): number {
    let counter: number = 0;
    while (counter < 2) {
        counter = counter + 1;
    }
    return counter; // Should be 2
}

function main(): number {
    return testSimpleWhile();
}