import SwiftUI
import CoreGraphics

// MARK: - Gradient Types

enum GradientType: String, CaseIterable {
    case linear = "linear"
    case radial = "radial"
    case angular = "angular"
    case diamond = "diamond"
    case conical = "conical"
    
    var name: String {
        switch self {
        case .linear: return "Linear"
        case .radial: return "Radial"
        case .angular: return "Angular"
        case .diamond: return "Diamond"
        case .conical: return "Conical"
        }
    }
    
    var icon: Image {
        switch self {
        case .linear: return Image(systemName: "arrow.left.and.right")
        case .radial: return Image(systemName: "circle")
        case .angular: return Image(systemName: "arrow.clockwise")
        case .diamond: return Image(systemName: "diamond")
        case .conical: return Image(systemName: "cone")
        }
    }
}

// MARK: - Gradient Model

class GradientElement: ObservableObject, Identifiable {
    let id = UUID()
    @Published var type: GradientType
    @Published var colors: [Color]
    @Published var locations: [Double]
    @Published var startPoint: CGPoint
    @Published var endPoint: CGPoint
    @Published var center: CGPoint
    @Published var radius: CGFloat
    @Published var angle: Double
    @Published var opacity: Double
    @Published var isSelected: Bool = false
    @Published var bounds: CGRect
    
    init(type: GradientType = .linear, colors: [Color] = [.blue, .purple]) {
        self.type = type
        self.colors = colors
        self.locations = [0.0, 1.0]
        self.startPoint = CGPoint(x: 0, y: 0)
        self.endPoint = CGPoint(x: 1, y: 1)
        self.center = CGPoint(x: 0.5, y: 0.5)
        self.radius = 0.5
        self.angle = 0
        self.opacity = 1.0
        self.bounds = CGRect(x: 0, y: 0, width: 400, height: 400)
    }
    
    func createGradient() -> LinearGradient? {
        guard colors.count >= 2 else { return nil }
        
        switch type {
        case .linear:
            return LinearGradient(
                colors: colors,
                startPoint: UnitPoint(x: startPoint.x, y: startPoint.y),
                endPoint: UnitPoint(x: endPoint.x, y: endPoint.y)
            )
        case .radial:
            return LinearGradient(
                colors: colors,
                startPoint: UnitPoint(x: center.x, y: center.y),
                endPoint: UnitPoint(x: center.x + radius, y: center.y + radius)
            )
        case .angular:
            return AngularGradient(
                colors: colors,
                center: UnitPoint(x: center.x, y: center.y),
                angle: .degrees(angle)
            )
        case .diamond:
            return LinearGradient(
                colors: colors,
                startPoint: UnitPoint(x: 0.5, y: 0),
                endPoint: UnitPoint(x: 0.5, y: 1)
            )
        case .conical:
            return AngularGradient(
                colors: colors,
                center: UnitPoint(x: center.x, y: center.y)
            )
        }
    }
}

// MARK: - Pattern Types

enum PatternType: String, CaseIterable {
    case dots = "dots"
    case stripes = "stripes"
    case checkerboard = "checkerboard"
    case hexagon = "hexagon"
    case stars = "stars"
    case hearts = "hearts"
    case flowers = "flowers"
    case waves = "waves"
    case bricks = "bricks"
    case wood = "wood"
    case marble = "marble"
    case fabric = "fabric"
    
    var name: String {
        switch self {
        case .dots: return "Dots"
        case .stripes: return "Stripes"
        case .checkerboard: return "Checkerboard"
        case .hexagon: return "Hexagon"
        case .stars: return "Stars"
        case .hearts: return "Hearts"
        case .flowers: return "Flowers"
        case .waves: return "Waves"
        case .bricks: return "Bricks"
        case .wood: return "Wood"
        case .marble: return "Marble"
        case .fabric: return "Fabric"
        }
    }
    
    var icon: Image {
        switch self {
        case .dots: return Image(systemName: "circle.fill")
        case .stripes: return Image(systemName: "line.3.horizontal")
        case .checkerboard: return Image(systemName: "square.grid.3x3")
        case .hexagon: return Image(systemName: "hexagon")
        case .stars: return Image(systemName: "star.fill")
        case .hearts: return Image(systemName: "heart.fill")
        case .flowers: return Image(systemName: "leaf.fill")
        case .waves: return Image(systemName: "waveform")
        case .bricks: return Image(systemName: "rectangle")
        case .wood: return Image(systemName: "tree")
        case .marble: return Image(systemName: "circle.dotted")
        case .fabric: return Image(systemName: "rectangle.portrait")
        }
    }
}

// MARK: - Pattern Model

class PatternElement: ObservableObject, Identifiable {
    let id = UUID()
    @Published var type: PatternType
    @Published var primaryColor: Color
    @Published var secondaryColor: Color
    @Published var scale: CGFloat
    @Published var rotation: Double
    @Published var opacity: Double
    @Published var isSelected: Bool = false
    @Published var bounds: CGRect
    
    init(type: PatternType = .dots, primaryColor: Color = .blue, secondaryColor: Color = .white) {
        self.type = type
        self.primaryColor = primaryColor
        self.secondaryColor = secondaryColor
        self.scale = 1.0
        self.rotation = 0
        self.opacity = 1.0
        self.bounds = CGRect(x: 0, y: 0, width: 400, height: 400)
    }
}

// MARK: - Gradient Pattern Manager

class GradientPatternManager: ObservableObject {
    @Published var gradients: [GradientElement] = []
    @Published var patterns: [PatternElement] = []
    @Published var selectedGradient: GradientElement?
    @Published var selectedPattern: PatternElement?
    @Published var showGradientPanel = false
    @Published var showPatternPanel = false
    @Published var currentGradientType: GradientType = .linear
    @Published var currentPatternType: PatternType = .dots
    
    // Gradient settings
    @Published var gradientColors: [Color] = [.blue, .purple]
    @Published var gradientOpacity: Double = 1.0
    @Published var gradientAngle: Double = 0
    
    // Pattern settings
    @Published var patternPrimaryColor: Color = .blue
    @Published var patternSecondaryColor: Color = .white
    @Published var patternScale: CGFloat = 1.0
    @Published var patternOpacity: Double = 1.0
    
    func addGradient() {
        let gradient = GradientElement(type: currentGradientType, colors: gradientColors)
        gradient.opacity = gradientOpacity
        gradient.angle = gradientAngle
        gradients.append(gradient)
        selectedGradient = gradient
    }
    
    func addPattern() {
        let pattern = PatternElement(
            type: currentPatternType,
            primaryColor: patternPrimaryColor,
            secondaryColor: patternSecondaryColor
        )
        pattern.scale = patternScale
        pattern.opacity = patternOpacity
        patterns.append(pattern)
        selectedPattern = pattern
    }
    
    func deleteGradient(_ gradient: GradientElement) {
        gradients.removeAll { $0.id == gradient.id }
        if selectedGradient?.id == gradient.id {
            selectedGradient = nil
        }
    }
    
    func deletePattern(_ pattern: PatternElement) {
        patterns.removeAll { $0.id == pattern.id }
        if selectedPattern?.id == pattern.id {
            selectedPattern = nil
        }
    }
    
    func clearAllGradients() {
        gradients.removeAll()
        selectedGradient = nil
    }
    
    func clearAllPatterns() {
        patterns.removeAll()
        selectedPattern = nil
    }
}

// MARK: - Gradient Panel View

struct GradientPanelView: View {
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🌈 Gradients")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        gradientPatternManager.showGradientPanel.toggle()
                    }
                }) {
                    Image(systemName: gradientPatternManager.showGradientPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if gradientPatternManager.showGradientPanel {
                VStack(spacing: 15) {
                    // Gradient type selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(GradientType.allCases, id: \.self) { type in
                                Button(action: {
                                    gradientPatternManager.currentGradientType = type
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        type.icon
                                            .font(.system(size: 25))
                                            .foregroundColor(gradientPatternManager.currentGradientType == type ? .white : .primary)
                                            .frame(width: 50, height: 50)
                                            .background(
                                                Circle()
                                                    .fill(gradientPatternManager.currentGradientType == type ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        
                                        Text(type.name)
                                            .font(.caption)
                                            .foregroundColor(.primary)
                                    }
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Color stops
                    VStack {
                        Text("Color Stops")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 6), spacing: 8) {
                            ForEach(Array(gradientPatternManager.gradientColors.enumerated()), id: \.offset) { index, color in
                                Button(action: {
                                    // Color picker would open here
                                }) {
                                    Circle()
                                        .fill(color)
                                        .frame(width: 40, height: 40)
                                        .overlay(
                                            Circle()
                                                .stroke(Color.black, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        
                        HStack {
                            Button("Add Color") {
                                gradientPatternManager.gradientColors.append(.green)
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Remove Color") {
                                if gradientPatternManager.gradientColors.count > 2 {
                                    gradientPatternManager.gradientColors.removeLast()
                                }
                            }
                            .buttonStyle(.bordered)
                            .disabled(gradientPatternManager.gradientColors.count <= 2)
                        }
                    }
                    
                    // Gradient preview
                    VStack {
                        Text("Preview")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Rectangle()
                            .fill(
                                LinearGradient(
                                    colors: gradientPatternManager.gradientColors,
                                    startPoint: .leading,
                                    endPoint: .trailing
                                )
                            )
                            .frame(height: 60)
                            .cornerRadius(10)
                    }
                    
                    // Gradient settings
                    VStack {
                        HStack {
                            Text("Opacity: \(Int(gradientPatternManager.gradientOpacity * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(gradientPatternManager.gradientOpacity * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $gradientPatternManager.gradientOpacity, in: 0.1...1.0)
                            .accentColor(.blue)
                    }
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button("Add Gradient") {
                            gradientPatternManager.addGradient()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.borderedProminent)
                        
                        Button("Clear All") {
                            gradientPatternManager.clearAllGradients()
                            SafeHapticFeedback.shared.clear()
                        }
                        .buttonStyle(.bordered)
                    }
                }
                .padding()
                .background(
                    RoundedRectangle(cornerRadius: 15)
                        .fill(Color.gray.opacity(0.1))
                )
                .padding(.horizontal)
            }
        }
        .padding(.vertical, 10)
        .background(
            LinearGradient(
                colors: [Color.blue.opacity(0.05), Color.purple.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Pattern Panel View

struct PatternPanelView: View {
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🎨 Patterns")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        gradientPatternManager.showPatternPanel.toggle()
                    }
                }) {
                    Image(systemName: gradientPatternManager.showPatternPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if gradientPatternManager.showPatternPanel {
                VStack(spacing: 15) {
                    // Pattern type selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(PatternType.allCases, id: \.self) { type in
                                Button(action: {
                                    gradientPatternManager.currentPatternType = type
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        type.icon
                                            .font(.system(size: 25))
                                            .foregroundColor(gradientPatternManager.currentPatternType == type ? .white : .primary)
                                            .frame(width: 50, height: 50)
                                            .background(
                                                Circle()
                                                    .fill(gradientPatternManager.currentPatternType == type ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        
                                        Text(type.name)
                                            .font(.caption)
                                            .foregroundColor(.primary)
                                    }
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Pattern colors
                    VStack {
                        Text("Pattern Colors")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack(spacing: 20) {
                            VStack {
                                Text("Primary")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Button(action: {
                                    // Color picker would open here
                                }) {
                                    Circle()
                                        .fill(gradientPatternManager.patternPrimaryColor)
                                        .frame(width: 50, height: 50)
                                        .overlay(
                                            Circle()
                                                .stroke(Color.black, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                            
                            VStack {
                                Text("Secondary")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Button(action: {
                                    // Color picker would open here
                                }) {
                                    Circle()
                                        .fill(gradientPatternManager.patternSecondaryColor)
                                        .frame(width: 50, height: 50)
                                        .overlay(
                                            Circle()
                                                .stroke(Color.black, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                    
                    // Pattern preview
                    VStack {
                        Text("Preview")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        PatternPreviewView(
                            type: gradientPatternManager.currentPatternType,
                            primaryColor: gradientPatternManager.patternPrimaryColor,
                            secondaryColor: gradientPatternManager.patternSecondaryColor,
                            scale: gradientPatternManager.patternScale
                        )
                        .frame(height: 60)
                        .cornerRadius(10)
                    }
                    
                    // Pattern settings
                    VStack {
                        HStack {
                            Text("Scale: \(Int(gradientPatternManager.patternScale * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(gradientPatternManager.patternScale * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $gradientPatternManager.patternScale, in: 0.5...3.0)
                            .accentColor(.green)
                    }
                    
                    VStack {
                        HStack {
                            Text("Opacity: \(Int(gradientPatternManager.patternOpacity * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(gradientPatternManager.patternOpacity * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $gradientPatternManager.patternOpacity, in: 0.1...1.0)
                            .accentColor(.blue)
                    }
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button("Add Pattern") {
                            gradientPatternManager.addPattern()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.borderedProminent)
                        
                        Button("Clear All") {
                            gradientPatternManager.clearAllPatterns()
                            SafeHapticFeedback.shared.clear()
                        }
                        .buttonStyle(.bordered)
                    }
                }
                .padding()
                .background(
                    RoundedRectangle(cornerRadius: 15)
                        .fill(Color.gray.opacity(0.1))
                )
                .padding(.horizontal)
            }
        }
        .padding(.vertical, 10)
        .background(
            LinearGradient(
                colors: [Color.green.opacity(0.05), Color.orange.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Pattern Preview View

struct PatternPreviewView: View {
    let type: PatternType
    let primaryColor: Color
    let secondaryColor: Color
    let scale: CGFloat
    
    var body: some View {
        Rectangle()
            .fill(patternFill)
            .scaleEffect(scale)
    }
    
    private var patternFill: AnyShapeStyle {
        switch type {
        case .dots:
            return AnyShapeStyle(
                RadialGradient(
                    colors: [primaryColor, secondaryColor],
                    center: .center,
                    startRadius: 0,
                    endRadius: 5
                )
            )
        case .stripes:
            return AnyShapeStyle(
                LinearGradient(
                    colors: [primaryColor, secondaryColor],
                    startPoint: .leading,
                    endPoint: .trailing
                )
            )
        case .checkerboard:
            return AnyShapeStyle(primaryColor)
        case .hexagon:
            return AnyShapeStyle(primaryColor)
        case .stars:
            return AnyShapeStyle(primaryColor)
        case .hearts:
            return AnyShapeStyle(primaryColor)
        case .flowers:
            return AnyShapeStyle(primaryColor)
        case .waves:
            return AnyShapeStyle(primaryColor)
        case .bricks:
            return AnyShapeStyle(primaryColor)
        case .wood:
            return AnyShapeStyle(primaryColor)
        case .marble:
            return AnyShapeStyle(primaryColor)
        case .fabric:
            return AnyShapeStyle(primaryColor)
        }
    }
}

// MARK: - Gradient Element View

struct GradientElementView: View {
    @ObservedObject var gradientElement: GradientElement
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @State private var dragOffset: CGSize = .zero
    @State private var isDragging = false
    
    var body: some View {
        Rectangle()
            .fill(gradientElement.createGradient() ?? LinearGradient(colors: [.clear], startPoint: .leading, endPoint: .trailing))
            .opacity(gradientElement.opacity)
            .frame(width: gradientElement.bounds.width, height: gradientElement.bounds.height)
            .position(
                x: gradientElement.bounds.midX + dragOffset.width,
                y: gradientElement.bounds.midY + dragOffset.height
            )
            .overlay(
                Rectangle()
                    .stroke(gradientElement.isSelected ? Color.blue : Color.clear, lineWidth: 2)
            )
            .onTapGesture {
                gradientPatternManager.selectedGradient = gradientElement
            }
            .gesture(
                DragGesture()
                    .onChanged { value in
                        if !isDragging {
                            isDragging = true
                        }
                        dragOffset = value.translation
                    }
                    .onEnded { value in
                        gradientElement.bounds = CGRect(
                            x: gradientElement.bounds.minX + value.translation.width,
                            y: gradientElement.bounds.minY + value.translation.height,
                            width: gradientElement.bounds.width,
                            height: gradientElement.bounds.height
                        )
                        dragOffset = .zero
                        isDragging = false
                    }
            )
    }
}

// MARK: - Pattern Element View

struct PatternElementView: View {
    @ObservedObject var patternElement: PatternElement
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @State private var dragOffset: CGSize = .zero
    @State private var isDragging = false
    
    var body: some View {
        Rectangle()
            .fill(patternFill)
            .opacity(patternElement.opacity)
            .frame(width: patternElement.bounds.width, height: patternElement.bounds.height)
            .position(
                x: patternElement.bounds.midX + dragOffset.width,
                y: patternElement.bounds.midY + dragOffset.height
            )
            .overlay(
                Rectangle()
                    .stroke(patternElement.isSelected ? Color.blue : Color.clear, lineWidth: 2)
            )
            .onTapGesture {
                gradientPatternManager.selectedPattern = patternElement
            }
            .gesture(
                DragGesture()
                    .onChanged { value in
                        if !isDragging {
                            isDragging = true
                        }
                        dragOffset = value.translation
                    }
                    .onEnded { value in
                        patternElement.bounds = CGRect(
                            x: patternElement.bounds.minX + value.translation.width,
                            y: patternElement.bounds.minY + value.translation.height,
                            width: patternElement.bounds.width,
                            height: patternElement.bounds.height
                        )
                        dragOffset = .zero
                        isDragging = false
                    }
            )
    }
    
    private var patternFill: AnyShapeStyle {
        switch patternElement.type {
        case .dots:
            return AnyShapeStyle(patternElement.primaryColor)
        case .stripes:
            return AnyShapeStyle(patternElement.primaryColor)
        case .checkerboard:
            return AnyShapeStyle(patternElement.primaryColor)
        case .hexagon:
            return AnyShapeStyle(patternElement.primaryColor)
        case .stars:
            return AnyShapeStyle(patternElement.primaryColor)
        case .hearts:
            return AnyShapeStyle(patternElement.primaryColor)
        case .flowers:
            return AnyShapeStyle(patternElement.primaryColor)
        case .waves:
            return AnyShapeStyle(patternElement.primaryColor)
        case .bricks:
            return AnyShapeStyle(patternElement.primaryColor)
        case .wood:
            return AnyShapeStyle(patternElement.primaryColor)
        case .marble:
            return AnyShapeStyle(patternElement.primaryColor)
        case .fabric:
            return AnyShapeStyle(patternElement.primaryColor)
        }
    }
}

// MARK: - Gradient Presets

struct GradientPresets {
    static let sunset = GradientElement(type: .linear, colors: [.orange, .pink, .purple])
    static let ocean = GradientElement(type: .linear, colors: [.blue, .cyan, .white])
    static let forest = GradientElement(type: .linear, colors: [.green, .mint, .white])
    static let fire = GradientElement(type: .radial, colors: [.red, .orange, .yellow])
    static let rainbow = GradientElement(type: .linear, colors: [.red, .orange, .yellow, .green, .blue, .purple])
    static let galaxy = GradientElement(type: .radial, colors: [.black, .purple, .blue])
    static let gold = GradientElement(type: .linear, colors: [.yellow, .orange])
    static let silver = GradientElement(type: .linear, colors: [.gray, .white])
    
    static let allPresets: [GradientElement] = [
        sunset, ocean, forest, fire, rainbow, galaxy, gold, silver
    ]
}

// MARK: - Pattern Presets

struct PatternPresets {
    static let polkaDots = PatternElement(type: .dots, primaryColor: .red, secondaryColor: .white)
    static let stripes = PatternElement(type: .stripes, primaryColor: .blue, secondaryColor: .white)
    static let checkerboard = PatternElement(type: .checkerboard, primaryColor: .black, secondaryColor: .white)
    static let stars = PatternElement(type: .stars, primaryColor: .yellow, secondaryColor: .blue)
    static let hearts = PatternElement(type: .hearts, primaryColor: .pink, secondaryColor: .white)
    static let flowers = PatternElement(type: .flowers, primaryColor: .green, secondaryColor: .white)
    
    static let allPresets: [PatternElement] = [
        polkaDots, stripes, checkerboard, stars, hearts, flowers
    ]
}