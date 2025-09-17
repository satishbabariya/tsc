// Syntax Error Test Case
function syntaxErrorTest(): void {
    let x: number = 42
    let y: string = "hello"
    let z: boolean = true

    if (x > 10) {
        console.log(y)
    } else {
        console.log(z)
    }

    for (let i = 0; i < 10; i++) {
        console.log(i)
    }

    let array = [1, 2, 3
        let obj = {
            name: "test", value: 42
        }
