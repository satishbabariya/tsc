// Complex nested objects with spread operations
interface BaseObject {
    id: number;
    name: string;
    properties: {
        color: string;
        size: string;
        dimensions: {
            width: number;
            height: number;
            depth: number;
        };
    };
    metadata: {
        created: string;
        updated: string;
        tags: string[];
    };
}

function createComplexNestedObject(): BaseObject {
    let baseObject: BaseObject = {
        id: 1,
        name: "base",
        properties: {
            color: "red",
            size: "large",
            dimensions: {
                width: 100,
                height: 200,
                depth: 50
            }
        },
        metadata: {
            created: "2024-01-01",
            updated: "2024-01-02",
            tags: ["important", "urgent", "critical"]
        }
    };

    // Complex spread operations with conflicts
    let extendedObject = {
        ...baseObject,
        ...baseObject.properties,
        ...baseObject.properties.dimensions,
        ...baseObject.metadata,
        additional: {
            ...baseObject.properties,
            ...baseObject.metadata,
            nested: {
                ...baseObject.properties.dimensions,
                ...baseObject.metadata.tags,
                conflict: "resolved"
            }
        },
        conflicts: {
            ...baseObject.properties,
            ...baseObject.metadata,
            name: "conflict", // This should conflict with baseObject.name
            color: "blue",    // This should conflict with baseObject.properties.color
            width: 999        // This should conflict with baseObject.properties.dimensions.width
        }
    };

    return extendedObject as BaseObject;
}

// Deep nested object with multiple spread levels
function createDeepNestedObject(): any {
    let level1 = {
        a: 1,
        b: {c: 2, d: 3},
        e: {f: {g: 4, h: 5}, i: 6}
    };

    let level2 = {
        ...level1,
        ...level1.b,
        ...level1.e,
        ...level1.e.f,
        nested: {
            ...level1,
            ...level1.b,
            ...level1.e,
            ...level1.e.f,
            deeper: {
                ...level1,
                ...level1.b,
                ...level1.e,
                ...level1.e.f,
                deepest: {
                    ...level1,
                    ...level1.b,
                    ...level1.e,
                    ...level1.e.f
                }
            }
        }
    };

    return level2;
}
