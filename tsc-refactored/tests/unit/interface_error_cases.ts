// Interface error cases (should fail)
interface InvalidInterface {
    // Missing semicolon - should error
    name: string
    age: number;
}

interface DuplicateProperty {
    id: string;
    id: number;  // Duplicate property - should error
}

interface InvalidMethod {
    // Missing return type - should error
    process(data: string)
}

interface InvalidInheritance {
    // Invalid extends - should error
    extends
    NonExistentInterface
}