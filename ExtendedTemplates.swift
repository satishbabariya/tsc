import SwiftUI

// MARK: - Extended Coloring Templates

enum ExtendedColoringTemplate: CaseIterable {
    case butterfly, flower, house, star, heart, cat
    case dog, bird, fish, tree, car, airplane
    case sun, moon, cloud, rainbow, apple, banana
    
    var name: String {
        switch self {
        case .butterfly: return "Butterfly"
        case .flower: return "Flower"
        case .house: return "House"
        case .star: return "Star"
        case .heart: return "Heart"
        case .cat: return "Cat"
        case .dog: return "Dog"
        case .bird: return "Bird"
        case .fish: return "Fish"
        case .tree: return "Tree"
        case .car: return "Car"
        case .airplane: return "Airplane"
        case .sun: return "Sun"
        case .moon: return "Moon"
        case .cloud: return "Cloud"
        case .rainbow: return "Rainbow"
        case .apple: return "Apple"
        case .banana: return "Banana"
        }
    }
    
    var icon: Image {
        switch self {
        case .butterfly: return Image(systemName: "butterfly")
        case .flower: return Image(systemName: "leaf")
        case .house: return Image(systemName: "house")
        case .star: return Image(systemName: "star")
        case .heart: return Image(systemName: "heart")
        case .cat: return Image(systemName: "cat")
        case .dog: return Image(systemName: "dog")
        case .bird: return Image(systemName: "bird")
        case .fish: return Image(systemName: "fish")
        case .tree: return Image(systemName: "tree")
        case .car: return Image(systemName: "car")
        case .airplane: return Image(systemName: "airplane")
        case .sun: return Image(systemName: "sun.max")
        case .moon: return Image(systemName: "moon")
        case .cloud: return Image(systemName: "cloud")
        case .rainbow: return Image(systemName: "cloud.rainbow")
        case .apple: return Image(systemName: "apple")
        case .banana: return Image(systemName: "banana")
        }
    }
    
    var category: TemplateCategory {
        switch self {
        case .butterfly, .bird, .fish, .cat, .dog:
            return .animals
        case .flower, .tree, .apple, .banana:
            return .nature
        case .car, .airplane:
            return .vehicles
        case .house:
            return .buildings
        case .sun, .moon, .cloud, .rainbow:
            return .weather
        case .star, .heart:
            return .shapes
        }
    }
    
    var drawing: Path {
        switch self {
        case .dog: return DogTemplate().path
        case .bird: return BirdTemplate().path
        case .fish: return FishTemplate().path
        case .tree: return TreeTemplate().path
        case .car: return CarTemplate().path
        case .airplane: return AirplaneTemplate().path
        case .sun: return SunTemplate().path
        case .moon: return MoonTemplate().path
        case .cloud: return CloudTemplate().path
        case .rainbow: return RainbowTemplate().path
        case .apple: return AppleTemplate().path
        case .banana: return BananaTemplate().path
        default: return Path() // Use existing templates
        }
    }
    
    var regions: [ColoringRegion] {
        switch self {
        case .dog: return DogTemplate().regions
        case .bird: return BirdTemplate().regions
        case .fish: return FishTemplate().regions
        case .tree: return TreeTemplate().regions
        case .car: return CarTemplate().regions
        case .airplane: return AirplaneTemplate().regions
        case .sun: return SunTemplate().regions
        case .moon: return MoonTemplate().path
        case .cloud: return CloudTemplate().regions
        case .rainbow: return RainbowTemplate().regions
        case .apple: return AppleTemplate().regions
        case .banana: return BananaTemplate().regions
        default: return [] // Use existing templates
        }
    }
}

enum TemplateCategory: String, CaseIterable {
    case animals = "Animals"
    case nature = "Nature"
    case vehicles = "Vehicles"
    case buildings = "Buildings"
    case weather = "Weather"
    case shapes = "Shapes"
    
    var icon: Image {
        switch self {
        case .animals: return Image(systemName: "pawprint")
        case .nature: return Image(systemName: "leaf")
        case .vehicles: return Image(systemName: "car")
        case .buildings: return Image(systemName: "building")
        case .weather: return Image(systemName: "cloud.sun")
        case .shapes: return Image(systemName: "circle")
        }
    }
}

// MARK: - New Template Implementations

struct DogTemplate {
    var path: Path {
        var path = Path()
        // Dog outline
        path.addEllipse(in: CGRect(x: 180, y: 200, width: 40, height: 30)) // head
        path.addEllipse(in: CGRect(x: 190, y: 210, width: 8, height: 8))   // left eye
        path.addEllipse(in: CGRect(x: 202, y: 210, width: 8, height: 8))   // right eye
        path.addEllipse(in: CGRect(x: 195, y: 220, width: 10, height: 6))  // nose
        path.addEllipse(in: CGRect(x: 170, y: 230, width: 60, height: 40)) // body
        path.addEllipse(in: CGRect(x: 160, y: 240, width: 20, height: 30)) // left leg
        path.addEllipse(in: CGRect(x: 220, y: 240, width: 20, height: 30)) // right leg
        path.addEllipse(in: CGRect(x: 150, y: 200, width: 15, height: 25)) // left ear
        path.addEllipse(in: CGRect(x: 235, y: 200, width: 15, height: 25)) // right ear
        path.addEllipse(in: CGRect(x: 200, y: 150, width: 30, height: 20)) // tail
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "head", path: Path(ellipseIn: CGRect(x: 180, y: 200, width: 40, height: 30))),
            ColoringRegion(id: "body", path: Path(ellipseIn: CGRect(x: 170, y: 230, width: 60, height: 40))),
            ColoringRegion(id: "leftEar", path: Path(ellipseIn: CGRect(x: 150, y: 200, width: 15, height: 25))),
            ColoringRegion(id: "rightEar", path: Path(ellipseIn: CGRect(x: 235, y: 200, width: 15, height: 25))),
            ColoringRegion(id: "leftLeg", path: Path(ellipseIn: CGRect(x: 160, y: 240, width: 20, height: 30))),
            ColoringRegion(id: "rightLeg", path: Path(ellipseIn: CGRect(x: 220, y: 240, width: 20, height: 30))),
            ColoringRegion(id: "tail", path: Path(ellipseIn: CGRect(x: 200, y: 150, width: 30, height: 20)))
        ]
    }
}

struct BirdTemplate {
    var path: Path {
        var path = Path()
        // Bird outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 30, height: 25)) // body
        path.addEllipse(in: CGRect(x: 190, y: 190, width: 20, height: 15))  // left wing
        path.addEllipse(in: CGRect(x: 220, y: 190, width: 20, height: 15)) // right wing
        path.addEllipse(in: CGRect(x: 195, y: 205, width: 6, height: 6))   // eye
        path.addEllipse(in: CGRect(x: 180, y: 210, width: 8, height: 4))   // beak
        path.addEllipse(in: CGRect(x: 200, y: 225, width: 4, height: 8))   // left leg
        path.addEllipse(in: CGRect(x: 210, y: 225, width: 4, height: 8))   // right leg
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "body", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 30, height: 25))),
            ColoringRegion(id: "leftWing", path: Path(ellipseIn: CGRect(x: 190, y: 190, width: 20, height: 15))),
            ColoringRegion(id: "rightWing", path: Path(ellipseIn: CGRect(x: 220, y: 190, width: 20, height: 15))),
            ColoringRegion(id: "beak", path: Path(ellipseIn: CGRect(x: 180, y: 210, width: 8, height: 4))),
            ColoringRegion(id: "leftLeg", path: Path(ellipseIn: CGRect(x: 200, y: 225, width: 4, height: 8))),
            ColoringRegion(id: "rightLeg", path: Path(ellipseIn: CGRect(x: 210, y: 225, width: 4, height: 8)))
        ]
    }
}

struct FishTemplate {
    var path: Path {
        var path = Path()
        // Fish outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 60, height: 40))   // body
        path.addEllipse(in: CGRect(x: 190, y: 210, width: 15, height: 20)) // left fin
        path.addEllipse(in: CGRect(x: 255, y: 210, width: 15, height: 20)) // right fin
        path.addEllipse(in: CGRect(x: 220, y: 205, width: 8, height: 8))   // eye
        path.addEllipse(in: CGRect(x: 240, y: 220, width: 20, height: 8))  // tail
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "body", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 60, height: 40))),
            ColoringRegion(id: "leftFin", path: Path(ellipseIn: CGRect(x: 190, y: 210, width: 15, height: 20))),
            ColoringRegion(id: "rightFin", path: Path(ellipseIn: CGRect(x: 255, y: 210, width: 15, height: 20))),
            ColoringRegion(id: "tail", path: Path(ellipseIn: CGRect(x: 240, y: 220, width: 20, height: 8)))
        ]
    }
}

struct TreeTemplate {
    var path: Path {
        var path = Path()
        // Tree outline
        path.addEllipse(in: CGRect(x: 200, y: 180, width: 80, height: 60))  // leaves
        path.addRect(CGRect(x: 230, y: 240, width: 20, height: 40))       // trunk
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 80, height: 40)) // main leaves
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "leaves", path: Path(ellipseIn: CGRect(x: 200, y: 180, width: 80, height: 60))),
            ColoringRegion(id: "trunk", path: Path(rect: CGRect(x: 230, y: 240, width: 20, height: 40)))
        ]
    }
}

struct CarTemplate {
    var path: Path {
        var path = Path()
        // Car outline
        path.addRect(CGRect(x: 180, y: 220, width: 80, height: 30))        // body
        path.addEllipse(in: CGRect(x: 190, y: 240, width: 20, height: 20)) // left wheel
        path.addEllipse(in: CGRect(x: 230, y: 240, width: 20, height: 20)) // right wheel
        path.addRect(CGRect(x: 200, y: 210, width: 40, height: 20))        // windshield
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "body", path: Path(rect: CGRect(x: 180, y: 220, width: 80, height: 30))),
            ColoringRegion(id: "leftWheel", path: Path(ellipseIn: CGRect(x: 190, y: 240, width: 20, height: 20))),
            ColoringRegion(id: "rightWheel", path: Path(ellipseIn: CGRect(x: 230, y: 240, width: 20, height: 20))),
            ColoringRegion(id: "windshield", path: Path(rect: CGRect(x: 200, y: 210, width: 40, height: 20)))
        ]
    }
}

struct AirplaneTemplate {
    var path: Path {
        var path = Path()
        // Airplane outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 60, height: 20)) // body
        path.addEllipse(in: CGRect(x: 180, y: 190, width: 30, height: 15)) // left wing
        path.addEllipse(in: CGRect(x: 180, y: 215, width: 30, height: 15)) // right wing
        path.addEllipse(in: CGRect(x: 250, y: 205, width: 15, height: 10)) // tail
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "body", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 60, height: 20))),
            ColoringRegion(id: "leftWing", path: Path(ellipseIn: CGRect(x: 180, y: 190, width: 30, height: 15))),
            ColoringRegion(id: "rightWing", path: Path(ellipseIn: CGRect(x: 180, y: 215, width: 30, height: 15))),
            ColoringRegion(id: "tail", path: Path(ellipseIn: CGRect(x: 250, y: 205, width: 15, height: 10)))
        ]
    }
}

struct SunTemplate {
    var path: Path {
        var path = Path()
        // Sun outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 80, height: 80)) // main sun
        // Sun rays
        for i in 0..<8 {
            let angle = Double(i) * Double.pi / 4
            let x = 240 + 60 * cos(angle)
            let y = 240 + 60 * sin(angle)
            path.addEllipse(in: CGRect(x: x - 5, y: y - 5, width: 10, height: 10))
        }
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "sun", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 80, height: 80))),
            ColoringRegion(id: "rays", path: {
                var path = Path()
                for i in 0..<8 {
                    let angle = Double(i) * Double.pi / 4
                    let x = 240 + 60 * cos(angle)
                    let y = 240 + 60 * sin(angle)
                    path.addEllipse(in: CGRect(x: x - 5, y: y - 5, width: 10, height: 10))
                }
                return path
            }())
        ]
    }
}

struct MoonTemplate {
    var path: Path {
        var path = Path()
        // Moon outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 60, height: 60)) // main moon
        path.addEllipse(in: CGRect(x: 210, y: 210, width: 40, height: 40)) // crescent
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "moon", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 60, height: 60)))
        ]
    }
}

struct CloudTemplate {
    var path: Path {
        var path = Path()
        // Cloud outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 60, height: 40)) // main cloud
        path.addEllipse(in: CGRect(x: 180, y: 210, width: 40, height: 30)) // left part
        path.addEllipse(in: CGRect(x: 240, y: 210, width: 40, height: 30)) // right part
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "cloud", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 60, height: 40))),
            ColoringRegion(id: "leftPart", path: Path(ellipseIn: CGRect(x: 180, y: 210, width: 40, height: 30))),
            ColoringRegion(id: "rightPart", path: Path(ellipseIn: CGRect(x: 240, y: 210, width: 40, height: 30)))
        ]
    }
}

struct RainbowTemplate {
    var path: Path {
        var path = Path()
        // Rainbow outline
        for i in 0..<7 {
            let radius = 80 - Double(i) * 10
            let rect = CGRect(x: 200 - radius/2, y: 200 - radius/2, width: radius, height: radius)
            path.addEllipse(in: rect)
        }
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "red", path: Path(ellipseIn: CGRect(x: 120, y: 120, width: 160, height: 160))),
            ColoringRegion(id: "orange", path: Path(ellipseIn: CGRect(x: 130, y: 130, width: 140, height: 140))),
            ColoringRegion(id: "yellow", path: Path(ellipseIn: CGRect(x: 140, y: 140, width: 120, height: 120))),
            ColoringRegion(id: "green", path: Path(ellipseIn: CGRect(x: 150, y: 150, width: 100, height: 100))),
            ColoringRegion(id: "blue", path: Path(ellipseIn: CGRect(x: 160, y: 160, width: 80, height: 80))),
            ColoringRegion(id: "indigo", path: Path(ellipseIn: CGRect(x: 170, y: 170, width: 60, height: 60))),
            ColoringRegion(id: "violet", path: Path(ellipseIn: CGRect(x: 180, y: 180, width: 40, height: 40)))
        ]
    }
}

struct AppleTemplate {
    var path: Path {
        var path = Path()
        // Apple outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 40, height: 50)) // main apple
        path.addEllipse(in: CGRect(x: 205, y: 190, width: 10, height: 15)) // stem
        path.addEllipse(in: CGRect(x: 210, y: 195, width: 8, height: 8))  // leaf
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "apple", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 40, height: 50))),
            ColoringRegion(id: "stem", path: Path(ellipseIn: CGRect(x: 205, y: 190, width: 10, height: 15))),
            ColoringRegion(id: "leaf", path: Path(ellipseIn: CGRect(x: 210, y: 195, width: 8, height: 8)))
        ]
    }
}

struct BananaTemplate {
    var path: Path {
        var path = Path()
        // Banana outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 30, height: 60)) // main banana
        path.addEllipse(in: CGRect(x: 205, y: 190, width: 8, height: 12)) // stem
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "banana", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 30, height: 60))),
            ColoringRegion(id: "stem", path: Path(ellipseIn: CGRect(x: 205, y: 190, width: 8, height: 12)))
        ]
    }
}