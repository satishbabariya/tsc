import SwiftUI

struct ContentView: View {
    @State private var selectedTemplate: ColoringTemplate = .butterfly
    @State private var selectedColor: Color = .red
    @State private var showColorPicker = false
    @State private var animateColors = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Header with template selection
                TemplateSelectorView(selectedTemplate: $selectedTemplate)
                
                // Main coloring area
                ColoringCanvasView(
                    template: selectedTemplate,
                    selectedColor: $selectedColor
                )
                
                // Color palette
                ColorPaletteView(selectedColor: $selectedColor, animateColors: $animateColors)
            }
            .navigationTitle("🎨 Color Fun")
            .navigationBarTitleDisplayMode(.inline)
            .onAppear {
                withAnimation(.easeInOut(duration: 2).repeatForever(autoreverses: true)) {
                    animateColors = true
                }
            }
        }
        .preferredColorScheme(.light)
    }
}

struct TemplateSelectorView: View {
    @Binding var selectedTemplate: ColoringTemplate
    
    var body: some View {
        ScrollView(.horizontal, showsIndicators: false) {
            HStack(spacing: 15) {
                ForEach(ColoringTemplate.allCases, id: \.self) { template in
                    Button(action: {
                        withAnimation(.spring(response: 0.4, dampingFraction: 0.6)) {
                            selectedTemplate = template
                        }
                        
                        // Haptic feedback
                        let impactFeedback = UIImpactFeedbackGenerator(style: .medium)
                        impactFeedback.impactOccurred()
                    }) {
                        VStack(spacing: 5) {
                            template.icon
                                .font(.system(size: 40))
                                .foregroundColor(selectedTemplate == template ? .white : .primary)
                                .frame(width: 60, height: 60)
                                .background(
                                    Circle()
                                        .fill(selectedTemplate == template ? 
                                              LinearGradient(colors: [.blue, .purple], startPoint: .topLeading, endPoint: .bottomTrailing) :
                                              LinearGradient(colors: [Color.gray.opacity(0.2)], startPoint: .topLeading, endPoint: .bottomTrailing)
                                        )
                                )
                                .scaleEffect(selectedTemplate == template ? 1.1 : 1.0)
                                .shadow(color: selectedTemplate == template ? .blue.opacity(0.3) : .clear, radius: 5)
                            
                            Text(template.name)
                                .font(.caption)
                                .fontWeight(selectedTemplate == template ? .bold : .regular)
                                .foregroundColor(.primary)
                        }
                    }
                    .buttonStyle(PlainButtonStyle())
                }
            }
            .padding(.horizontal)
        }
        .padding(.vertical, 15)
        .background(
            LinearGradient(
                colors: [Color.blue.opacity(0.05), Color.purple.opacity(0.05)],
                startPoint: .leading,
                endPoint: .trailing
            )
        )
    }
}

struct ColoringCanvasView: View {
    let template: ColoringTemplate
    @Binding var selectedColor: Color
    @State private var coloredRegions: [String: Color] = [:]
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                // Template drawing
                template.drawing
                    .stroke(Color.black, lineWidth: 3)
                    .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                
                // Colored regions
                ForEach(template.regions, id: \.id) { region in
                    region.path
                        .fill(coloredRegions[region.id] ?? Color.clear)
                        .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                        .onTapGesture {
                            withAnimation(.easeInOut(duration: 0.3)) {
                                coloredRegions[region.id] = selectedColor
                            }
                        }
                }
            }
            .padding(20)
        }
        .onChange(of: template) { _ in
            coloredRegions.removeAll()
        }
    }
}

struct ColorPaletteView: View {
    @Binding var selectedColor: Color
    @Binding var animateColors: Bool
    
    let colors: [Color] = [
        .red, .orange, .yellow, .green, .mint, .teal,
        .cyan, .blue, .indigo, .purple, .pink, .brown,
        .gray, .black, .white, Color(red: 1, green: 0.8, blue: 0.8) // Light pink
    ]
    
    var body: some View {
        VStack(spacing: 15) {
            HStack {
                Text("🎨 Choose Your Color")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                // Animated sparkle
                Image(systemName: "sparkles")
                    .foregroundColor(.yellow)
                    .scaleEffect(animateColors ? 1.2 : 1.0)
                    .animation(.easeInOut(duration: 1).repeatForever(autoreverses: true), value: animateColors)
            }
            .padding(.horizontal)
            
            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 12) {
                ForEach(Array(colors.enumerated()), id: \.offset) { index, color in
                    Button(action: {
                        withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                            selectedColor = color
                        }
                        
                        // Haptic feedback
                        let impactFeedback = UIImpactFeedbackGenerator(style: .light)
                        impactFeedback.impactOccurred()
                    }) {
                        Circle()
                            .fill(color)
                            .frame(width: 45, height: 45)
                            .overlay(
                                Circle()
                                    .stroke(selectedColor == color ? Color.black : Color.clear, lineWidth: 4)
                            )
                            .scaleEffect(selectedColor == color ? 1.3 : 1.0)
                            .shadow(color: selectedColor == color ? color.opacity(0.5) : .clear, radius: 8)
                            .animation(.spring(response: 0.3, dampingFraction: 0.6).delay(Double(index) * 0.05), value: selectedColor)
                    }
                    .buttonStyle(PlainButtonStyle())
                }
            }
            .padding(.horizontal)
        }
        .padding(.vertical, 20)
        .background(
            LinearGradient(
                colors: [Color.yellow.opacity(0.1), Color.orange.opacity(0.1)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Coloring Templates

enum ColoringTemplate: CaseIterable {
    case butterfly, flower, house, star, heart, cat
    
    var name: String {
        switch self {
        case .butterfly: return "Butterfly"
        case .flower: return "Flower"
        case .house: return "House"
        case .star: return "Star"
        case .heart: return "Heart"
        case .cat: return "Cat"
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
        }
    }
    
    var drawing: Path {
        switch self {
        case .butterfly:
            return ButterflyTemplate().path
        case .flower:
            return FlowerTemplate().path
        case .house:
            return HouseTemplate().path
        case .star:
            return StarTemplate().path
        case .heart:
            return HeartTemplate().path
        case .cat:
            return CatTemplate().path
        }
    }
    
    var regions: [ColoringRegion] {
        switch self {
        case .butterfly:
            return ButterflyTemplate().regions
        case .flower:
            return FlowerTemplate().regions
        case .house:
            return HouseTemplate().regions
        case .star:
            return StarTemplate().regions
        case .heart:
            return HeartTemplate().regions
        case .cat:
            return CatTemplate().regions
        }
    }
}

struct ColoringRegion {
    let id: String
    let path: Path
}

// MARK: - Template Implementations

struct ButterflyTemplate {
    var path: Path {
        var path = Path()
        // Simplified butterfly outline
        path.move(to: CGPoint(x: 200, y: 200))
        path.addEllipse(in: CGRect(x: 150, y: 150, width: 100, height: 100))
        path.move(to: CGPoint(x: 200, y: 200))
        path.addEllipse(in: CGRect(x: 150, y: 250, width: 100, height: 100))
        path.move(to: CGPoint(x: 200, y: 200))
        path.addLine(to: CGPoint(x: 200, y: 350))
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "wing1", path: Path(ellipseIn: CGRect(x: 150, y: 150, width: 100, height: 100))),
            ColoringRegion(id: "wing2", path: Path(ellipseIn: CGRect(x: 150, y: 250, width: 100, height: 100))),
            ColoringRegion(id: "body", path: Path(rect: CGRect(x: 195, y: 200, width: 10, height: 150)))
        ]
    }
}

struct FlowerTemplate {
    var path: Path {
        var path = Path()
        // Flower outline
        path.addEllipse(in: CGRect(x: 200, y: 200, width: 200, height: 200))
        path.addEllipse(in: CGRect(x: 250, y: 200, width: 100, height: 100))
        path.addEllipse(in: CGRect(x: 150, y: 200, width: 100, height: 100))
        path.addEllipse(in: CGRect(x: 200, y: 150, width: 100, height: 100))
        path.addEllipse(in: CGRect(x: 200, y: 250, width: 100, height: 100))
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "center", path: Path(ellipseIn: CGRect(x: 200, y: 200, width: 200, height: 200))),
            ColoringRegion(id: "petal1", path: Path(ellipseIn: CGRect(x: 250, y: 200, width: 100, height: 100))),
            ColoringRegion(id: "petal2", path: Path(ellipseIn: CGRect(x: 150, y: 200, width: 100, height: 100))),
            ColoringRegion(id: "petal3", path: Path(ellipseIn: CGRect(x: 200, y: 150, width: 100, height: 100))),
            ColoringRegion(id: "petal4", path: Path(ellipseIn: CGRect(x: 200, y: 250, width: 100, height: 100)))
        ]
    }
}

struct HouseTemplate {
    var path: Path {
        var path = Path()
        // House outline
        path.move(to: CGPoint(x: 200, y: 300))
        path.addLine(to: CGPoint(x: 150, y: 250))
        path.addLine(to: CGPoint(x: 250, y: 250))
        path.addLine(to: CGPoint(x: 200, y: 300))
        path.addRect(CGRect(x: 170, y: 250, width: 60, height: 50))
        path.addRect(CGRect(x: 180, y: 260, width: 20, height: 20))
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "roof", path: {
                var path = Path()
                path.move(to: CGPoint(x: 200, y: 300))
                path.addLine(to: CGPoint(x: 150, y: 250))
                path.addLine(to: CGPoint(x: 250, y: 250))
                path.addLine(to: CGPoint(x: 200, y: 300))
                return path
            }()),
            ColoringRegion(id: "house", path: Path(rect: CGRect(x: 170, y: 250, width: 60, height: 50))),
            ColoringRegion(id: "door", path: Path(rect: CGRect(x: 180, y: 260, width: 20, height: 20)))
        ]
    }
}

struct StarTemplate {
    var path: Path {
        var path = Path()
        // 5-pointed star
        let center = CGPoint(x: 200, y: 200)
        let radius: CGFloat = 80
        
        for i in 0..<5 {
            let angle = Double(i) * 2 * Double.pi / 5 - Double.pi / 2
            let x = center.x + radius * cos(angle)
            let y = center.y + radius * sin(angle)
            
            if i == 0 {
                path.move(to: CGPoint(x: x, y: y))
            } else {
                path.addLine(to: CGPoint(x: x, y: y))
            }
        }
        path.closeSubpath()
        return path
    }
    
    var regions: [ColoringRegion] {
        [ColoringRegion(id: "star", path: path)]
    }
}

struct HeartTemplate {
    var path: Path {
        var path = Path()
        // Heart shape
        path.move(to: CGPoint(x: 200, y: 250))
        path.addCurve(to: CGPoint(x: 200, y: 200), control1: CGPoint(x: 180, y: 220), control2: CGPoint(x: 180, y: 200))
        path.addCurve(to: CGPoint(x: 200, y: 250), control1: CGPoint(x: 220, y: 200), control2: CGPoint(x: 220, y: 220))
        path.addCurve(to: CGPoint(x: 200, y: 300), control1: CGPoint(x: 180, y: 280), control2: CGPoint(x: 200, y: 300))
        path.addCurve(to: CGPoint(x: 200, y: 250), control1: CGPoint(x: 200, y: 300), control2: CGPoint(x: 220, y: 280))
        return path
    }
    
    var regions: [ColoringRegion] {
        [ColoringRegion(id: "heart", path: path)]
    }
}

struct CatTemplate {
    var path: Path {
        var path = Path()
        // Cat outline
        path.addEllipse(in: CGRect(x: 180, y: 200, width: 40, height: 30)) // head
        path.addEllipse(in: CGRect(x: 190, y: 210, width: 8, height: 8))   // left eye
        path.addEllipse(in: CGRect(x: 202, y: 210, width: 8, height: 8))   // right eye
        path.addEllipse(in: CGRect(x: 195, y: 220, width: 10, height: 6))  // nose
        path.addEllipse(in: CGRect(x: 170, y: 230, width: 60, height: 40)) // body
        path.addEllipse(in: CGRect(x: 160, y: 240, width: 20, height: 30)) // left leg
        path.addEllipse(in: CGRect(x: 220, y: 240, width: 20, height: 30)) // right leg
        path.addEllipse(in: CGRect(x: 150, y: 200, width: 15, height: 25)) // left ear
        path.addEllipse(in: CGRect(x: 235, y: 200, width: 15, height: 25)) // right ear
        return path
    }
    
    var regions: [ColoringRegion] {
        [
            ColoringRegion(id: "head", path: Path(ellipseIn: CGRect(x: 180, y: 200, width: 40, height: 30))),
            ColoringRegion(id: "body", path: Path(ellipseIn: CGRect(x: 170, y: 230, width: 60, height: 40))),
            ColoringRegion(id: "leftEar", path: Path(ellipseIn: CGRect(x: 150, y: 200, width: 15, height: 25))),
            ColoringRegion(id: "rightEar", path: Path(ellipseIn: CGRect(x: 235, y: 200, width: 15, height: 25))),
            ColoringRegion(id: "leftLeg", path: Path(ellipseIn: CGRect(x: 160, y: 240, width: 20, height: 30))),
            ColoringRegion(id: "rightLeg", path: Path(ellipseIn: CGRect(x: 220, y: 240, width: 20, height: 30)))
        ]
    }
}

#Preview {
    ContentView()
}