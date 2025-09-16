// Test object literals and property access
function testObjects() {
    // Object literals
    let person = {
        name: "John",
        age: 30,
        active: true
    };

    let empty = {};

    let nested = {
        user: {
            id: 123,
            profile: {
                email: "test@example.com"
            }
        }
    };

    // Property access
    let name = person.name;
    let age = person.age;
    let email = nested.user.profile.email;

    // Mixed access (property + array)
    let config = {
        items: [1, 2, 3],
        settings: {enabled: true}
    };

    let firstItem = config.items[0];
    let isEnabled = config.settings.enabled;

    return 42;
}