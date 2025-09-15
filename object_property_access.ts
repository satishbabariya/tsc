
// Object property access testing
interface Product {
    id: number;
    name: string;
    price: number;
    category: string;
    inStock: boolean;
}

function testObjectPropertyAccess() {
    const product: Product = {
        id: 1,
        name: "Laptop",
        price: 999.99,
        category: "Electronics",
        inStock: true
    };
    
    // Dot notation access
    console.log("Product name:", product.name);
    console.log("Product price:", product.price);
    console.log("Product category:", product.category);
    
    // Bracket notation access
    console.log("Product name (bracket):", product["name"]);
    console.log("Product price (bracket):", product["price"]);
    console.log("Product category (bracket):", product["category"]);
    
    // Dynamic property access
    const propertyName = "name";
    console.log("Dynamic property access:", product[propertyName]);
    
    // Computed property access
    const dynamicKey = "price";
    console.log("Computed property access:", product[`${dynamicKey}`]);
}

// Test with nested property access
function testNestedPropertyAccess() {
    const user = {
        id: 1,
        profile: {
            personal: {
                name: "Alice",
                age: 30
            },
            contact: {
                email: "alice@example.com",
                phone: "555-1234"
            }
        },
        settings: {
            theme: "dark",
            notifications: {
                email: true,
                push: false
            }
        }
    };
    
    // Nested dot notation
    console.log("User name:", user.profile.personal.name);
    console.log("User email:", user.profile.contact.email);
    console.log("User theme:", user.settings.theme);
    
    // Nested bracket notation
    console.log("User age:", user["profile"]["personal"]["age"]);
    console.log("User phone:", user["profile"]["contact"]["phone"]);
    
    // Mixed notation
    console.log("Email notifications:", user.settings["notifications"]["email"]);
    console.log("Push notifications:", user["settings"].notifications.push);
}

// Test with array-like objects
function testArrayLikePropertyAccess() {
    const arrayLike = {
        0: "first",
        1: "second",
        2: "third",
        length: 3
    };
    
    // Access like array
    console.log("First element:", arrayLike[0]);
    console.log("Second element:", arrayLike[1]);
    console.log("Third element:", arrayLike[2]);
    console.log("Length:", arrayLike.length);
    
    // Iterate like array
    for (let i = 0; i < arrayLike.length; i++) {
        console.log(`Element ${i}:`, arrayLike[i]);
    }
}

// Test with optional chaining simulation
function testOptionalPropertyAccess() {
    const user = {
        profile: {
            name: "Alice",
            address: {
                street: "123 Main St",
                city: "New York"
            }
        }
    };
    
    // Safe property access
    const userName = user.profile?.name;
    const userStreet = user.profile?.address?.street;
    const userCountry = user.profile?.address?.country;
    
    console.log("User name:", userName);
    console.log("User street:", userStreet);
    console.log("User country:", userCountry);
}

// Run tests
testObjectPropertyAccess();
testNestedPropertyAccess();
testArrayLikePropertyAccess();
testOptionalPropertyAccess();
