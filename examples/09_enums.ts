// Enums
// Demonstrates enum declarations, member access, and usage

// Basic numeric enum
enum Direction {
    Up,
    Down,
    Left,
    Right
}

// Enum with explicit values
enum Status {
    Pending = 0,
    Approved = 1,
    Rejected = 2,
    Cancelled = 3
}

// Enum with string values
enum Color {
    Red = "red",
    Green = "green",
    Blue = "blue",
    Yellow = "yellow"
}

// Enum with mixed values
enum MixedEnum {
    First = 1,
    Second = "second",
    Third = 3,
    Fourth = "fourth"
}

// Enum with computed values
enum FileSize {
    Small = 1024,
    Medium = Small * 2,
    Large = Medium * 2,
    ExtraLarge = Large * 2
}

// Enum with bit flags
enum Permission {
    None = 0,
    Read = 1,
    Write = 2,
    Execute = 4,
    All = Read | Write | Execute
}

// Enum with negative values
enum Temperature {
    Freezing = -10,
    Cold = 0,
    Warm = 20,
    Hot = 30
}

// Enum with decimal values
enum Precision {
    Low = 0.1,
    Medium = 0.01,
    High = 0.001,
    VeryHigh = 0.0001
}

// Enum with hexadecimal values
enum ErrorCode {
    Success = 0x00,
    InvalidInput = 0x01,
    NotFound = 0x02,
    AccessDenied = 0x03,
    InternalError = 0xFF
}

// Enum with binary values
enum BinaryFlag {
    Flag0 = 0b0001,
    Flag1 = 0b0010,
    Flag2 = 0b0100,
    Flag3 = 0b1000
}

// Enum with octal values
enum OctalValue {
    Value1 = 0o10,
    Value2 = 0o20,
    Value3 = 0o40,
    Value4 = 0o100
}

// Enum with scientific notation
enum ScientificValue {
    Micro = 1e-6,
    Milli = 1e-3,
    Kilo = 1e3,
    Mega = 1e6
}

// Enum with string interpolation (concept)
enum MessageType {
    Info = "INFO",
    Warning = "WARN",
    Error = "ERROR",
    Debug = "DEBUG"
}

// Enum with special characters
enum SpecialChars {
    Space = " ",
    Tab = "\t",
    Newline = "\n",
    CarriageReturn = "\r"
}

// Enum with unicode characters
enum UnicodeChars {
    Heart = "\u2665",
    Star = "\u2606",
    Check = "\u2713",
    Cross = "\u2717"
}

// Enum usage examples
let currentDirection: Direction = Direction.Up;
let currentStatus: Status = Status.Pending;
let favoriteColor: Color = Color.Blue;

// Enum member access
let upValue: number = Direction.Up;        // 0
let downValue: number = Direction.Down;    // 1
let leftValue: number = Direction.Left;    // 2
let rightValue: number = Direction.Right;  // 3

let redValue: string = Color.Red;          // "red"
let greenValue: string = Color.Green;      // "green"
let blueValue: string = Color.Blue;        // "blue"

// Enum comparison
if (currentDirection === Direction.Up) {
    // Handle up direction
}

if (currentStatus === Status.Approved) {
    // Handle approved status
}

// Enum in switch statements
function getDirectionName(direction: Direction): string {
    switch (direction) {
        case Direction.Up:
            return "Up";
        case Direction.Down:
            return "Down";
        case Direction.Left:
            return "Left";
        case Direction.Right:
            return "Right";
        default:
            return "Unknown";
    }
}

function getStatusMessage(status: Status): string {
    switch (status) {
        case Status.Pending:
            return "Request is pending approval";
        case Status.Approved:
            return "Request has been approved";
        case Status.Rejected:
            return "Request has been rejected";
        case Status.Cancelled:
            return "Request has been cancelled";
        default:
            return "Unknown status";
    }
}

// Enum with bit operations
let userPermissions: Permission = Permission.Read | Permission.Write;
let hasReadPermission: boolean = (userPermissions & Permission.Read) !== 0;
let hasWritePermission: boolean = (userPermissions & Permission.Write) !== 0;
let hasExecutePermission: boolean = (userPermissions & Permission.Execute) !== 0;

// Enum iteration (manual implementation)
function getAllDirections(): Direction[] {
    return [Direction.Up, Direction.Down, Direction.Left, Direction.Right];
}

function getAllColors(): Color[] {
    return [Color.Red, Color.Green, Color.Blue, Color.Yellow];
}

// Enum with methods (using objects)
let DirectionUtils = {
    Up: Direction.Up,
    Down: Direction.Down,
    Left: Direction.Left,
    Right: Direction.Right,
    
    isHorizontal: function(direction: Direction): boolean {
        return direction === Direction.Left || direction === Direction.Right;
    },
    
    isVertical: function(direction: Direction): boolean {
        return direction === Direction.Up || direction === Direction.Down;
    },
    
    opposite: function(direction: Direction): Direction {
        switch (direction) {
            case Direction.Up: return Direction.Down;
            case Direction.Down: return Direction.Up;
            case Direction.Left: return Direction.Right;
            case Direction.Right: return Direction.Left;
            default: return direction;
        }
    }
};

// Using enum utilities
let isHoriz: boolean = DirectionUtils.isHorizontal(Direction.Left);
let isVert: boolean = DirectionUtils.isVertical(Direction.Up);
let oppositeDir: Direction = DirectionUtils.opposite(Direction.Right);

// Enum with string methods
let MessageUtils = {
    Info: MessageType.Info,
    Warning: MessageType.Warning,
    Error: MessageType.Error,
    Debug: MessageType.Debug,
    
    formatMessage: function(type: MessageType, message: string): string {
        return "[" + type + "] " + message;
    },
    
    isError: function(type: MessageType): boolean {
        return type === MessageType.Error;
    }
};

let formattedMessage: string = MessageUtils.formatMessage(MessageType.Info, "Application started");
let isError: boolean = MessageUtils.isError(MessageType.Error);

// Enum with numeric operations
let fileSize: FileSize = FileSize.Small;
let sizeInBytes: number = fileSize;
let isLargeFile: boolean = fileSize >= FileSize.Large;

// Enum with error codes
let errorCode: ErrorCode = ErrorCode.Success;
let isSuccess: boolean = errorCode === ErrorCode.Success;
let isErrorCode: boolean = errorCode !== ErrorCode.Success;

// Enum with temperature ranges
let temp: Temperature = Temperature.Warm;
let isCold: boolean = temp <= Temperature.Cold;
let isHot: boolean = temp >= Temperature.Hot;

// Enum with precision levels
let precision: Precision = Precision.Medium;
let isHighPrecision: boolean = precision <= Precision.High;

// Enum with binary flags
let flags: BinaryFlag = BinaryFlag.Flag0 | BinaryFlag.Flag2;
let hasFlag0: boolean = (flags & BinaryFlag.Flag0) !== 0;
let hasFlag1: boolean = (flags & BinaryFlag.Flag1) !== 0;

// This example demonstrates comprehensive enum functionality