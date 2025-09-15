import SwiftUI
import CoreGraphics

// MARK: - Transformation Types

enum TransformationType: String, CaseIterable {
    case move = "move"
    case rotate = "rotate"
    case scale = "scale"
    case flipHorizontal = "flip_horizontal"
    case flipVertical = "flip_vertical"
    case skew = "skew"
    case perspective = "perspective"
    
    var name: String {
        switch self {
        case .move: return "Move"
        case .rotate: return "Rotate"
        case .scale: return "Scale"
        case .flipHorizontal: return "Flip Horizontal"
        case .flipVertical: return "Flip Vertical"
        case .skew: return "Skew"
        case .perspective: return "Perspective"
        }
    }
    
    var icon: Image {
        switch self {
        case .move: return Image(systemName: "arrow.up.and.down.and.arrow.left.and.right")
        case .rotate: return Image(systemName: "rotate.right")
        case .scale: return Image(systemName: "arrow.up.left.and.arrow.down.right")
        case .flipHorizontal: return Image(systemName: "arrow.left.and.right")
        case .flipVertical: return Image(systemName: "arrow.up.and.down")
        case .skew: return Image(systemName: "arrow.up.left.and.down.right.and.arrow.up.right.and.down.left")
        case .perspective: return Image(systemName: "view.3d")
        }
    }
}

// MARK: - Transformation Model

struct Transformation: Identifiable {
    let id = UUID()
    let type: TransformationType
    let value: Double
    let center: CGPoint
    let timestamp: Date
    
    init(type: TransformationType, value: Double, center: CGPoint = CGPoint(x: 200, y: 200)) {
        self.type = type
        self.value = value
        self.center = center
        self.timestamp = Date()
    }
}

// MARK: - Transformation Manager

class TransformationManager: ObservableObject {
    @Published var currentTool: TransformationType = .move
    @Published var transformations: [Transformation] = []
    @Published var isTransforming = false
    @Published var showTransformationPanel = false
    @Published var selectedLayer: DrawingLayer?
    @Published var transformCenter: CGPoint = CGPoint(x: 200, y: 200)
    
    // Transformation values
    @Published var rotationAngle: Double = 0
    @Published var scaleX: Double = 1.0
    @Published var scaleY: Double = 1.0
    @Published var skewX: Double = 0
    @Published var skewY: Double = 0
    @Published var perspectiveX: Double = 0
    @Published var perspectiveY: Double = 0
    
    func startTransformation(of layer: DrawingLayer) {
        selectedLayer = layer
        isTransforming = true
        transformCenter = calculateLayerCenter(layer)
    }
    
    func applyTransformation() {
        guard let layer = selectedLayer else { return }
        
        let transformation = Transformation(
            type: currentTool,
            value: getCurrentValue(),
            center: transformCenter
        )
        
        transformations.append(transformation)
        applyTransformationToLayer(transformation)
        
        isTransforming = false
    }
    
    func cancelTransformation() {
        isTransforming = false
        resetValues()
    }
    
    func resetValues() {
        rotationAngle = 0
        scaleX = 1.0
        scaleY = 1.0
        skewX = 0
        skewY = 0
        perspectiveX = 0
        perspectiveY = 0
    }
    
    private func getCurrentValue() -> Double {
        switch currentTool {
        case .move: return 0
        case .rotate: return rotationAngle
        case .scale: return scaleX
        case .flipHorizontal, .flipVertical: return 1.0
        case .skew: return skewX
        case .perspective: return perspectiveX
        }
    }
    
    private func calculateLayerCenter(_ layer: DrawingLayer) -> CGPoint {
        // Calculate center point of layer's strokes
        guard !layer.strokes.isEmpty else { return CGPoint(x: 200, y: 200) }
        
        var minX: CGFloat = .infinity
        var maxX: CGFloat = -.infinity
        var minY: CGFloat = .infinity
        var maxY: CGFloat = -.infinity
        
        for stroke in layer.strokes {
            for point in stroke.points {
                minX = min(minX, point.x)
                maxX = max(maxX, point.x)
                minY = min(minY, point.y)
                maxY = max(maxY, point.y)
            }
        }
        
        return CGPoint(
            x: (minX + maxX) / 2,
            y: (minY + maxY) / 2
        )
    }
    
    private func applyTransformationToLayer(_ transformation: Transformation) {
        guard let layer = selectedLayer else { return }
        
        switch transformation.type {
        case .move:
            moveLayer(layer, by: CGPoint(x: transformation.value, y: transformation.value))
        case .rotate:
            rotateLayer(layer, by: transformation.value, around: transformation.center)
        case .scale:
            scaleLayer(layer, by: transformation.value, around: transformation.center)
        case .flipHorizontal:
            flipLayerHorizontally(layer, around: transformation.center)
        case .flipVertical:
            flipLayerVertically(layer, around: transformation.center)
        case .skew:
            skewLayer(layer, by: transformation.value)
        case .perspective:
            applyPerspectiveToLayer(layer, by: transformation.value)
        }
    }
    
    private func moveLayer(_ layer: DrawingLayer, by offset: CGPoint) {
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                CGPoint(x: point.x + offset.x, y: point.y + offset.y)
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
    
    private func rotateLayer(_ layer: DrawingLayer, by angle: Double, around center: CGPoint) {
        let radians = angle * .pi / 180
        
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                let dx = point.x - center.x
                let dy = point.y - center.y
                
                let rotatedX = dx * cos(radians) - dy * sin(radians)
                let rotatedY = dx * sin(radians) + dy * cos(radians)
                
                return CGPoint(x: rotatedX + center.x, y: rotatedY + center.y)
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
    
    private func scaleLayer(_ layer: DrawingLayer, by scale: Double, around center: CGPoint) {
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                let dx = point.x - center.x
                let dy = point.y - center.y
                
                return CGPoint(
                    x: dx * scale + center.x,
                    y: dy * scale + center.y
                )
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
    
    private func flipLayerHorizontally(_ layer: DrawingLayer, around center: CGPoint) {
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                let dx = point.x - center.x
                return CGPoint(x: center.x - dx, y: point.y)
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
    
    private func flipLayerVertically(_ layer: DrawingLayer, around center: CGPoint) {
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                let dy = point.y - center.y
                return CGPoint(x: point.x, y: center.y - dy)
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
    
    private func skewLayer(_ layer: DrawingLayer, by skew: Double) {
        let radians = skew * .pi / 180
        
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                return CGPoint(
                    x: point.x + point.y * tan(radians),
                    y: point.y
                )
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
    
    private func applyPerspectiveToLayer(_ layer: DrawingLayer, by perspective: Double) {
        // Simplified perspective transformation
        let factor = perspective / 1000.0
        
        for i in 0..<layer.strokes.count {
            var stroke = layer.strokes[i]
            stroke.points = stroke.points.map { point in
                let z = 1.0 + factor * (point.y / 400.0)
                return CGPoint(
                    x: point.x / z,
                    y: point.y / z
                )
            }
            layer.strokes[i] = stroke
        }
        layer.generateThumbnail()
    }
}

// MARK: - Transformation Tool Palette

struct TransformationToolPalette: View {
    @ObservedObject var transformationManager: TransformationManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🔄 Transform Tools")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        transformationManager.showTransformationPanel.toggle()
                    }
                }) {
                    Image(systemName: transformationManager.showTransformationPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if transformationManager.showTransformationPanel {
                VStack(spacing: 15) {
                    // Tool selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(TransformationType.allCases, id: \.self) { tool in
                                Button(action: {
                                    transformationManager.currentTool = tool
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        tool.icon
                                            .font(.system(size: 25))
                                            .foregroundColor(transformationManager.currentTool == tool ? .white : .primary)
                                            .frame(width: 50, height: 50)
                                            .background(
                                                Circle()
                                                    .fill(transformationManager.currentTool == tool ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        
                                        Text(tool.name)
                                            .font(.caption)
                                            .foregroundColor(.primary)
                                    }
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Transformation controls
                    TransformationControlsView(transformationManager: transformationManager)
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button("Apply") {
                            transformationManager.applyTransformation()
                            SafeHapticFeedback.shared.success()
                        }
                        .buttonStyle(.borderedProminent)
                        .disabled(!transformationManager.isTransforming)
                        
                        Button("Cancel") {
                            transformationManager.cancelTransformation()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.bordered)
                        .disabled(!transformationManager.isTransforming)
                        
                        Button("Reset") {
                            transformationManager.resetValues()
                            SafeHapticFeedback.shared.lightImpact()
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
                colors: [Color.green.opacity(0.05), Color.blue.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Transformation Controls View

struct TransformationControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    
    var body: some View {
        VStack(spacing: 15) {
            switch transformationManager.currentTool {
            case .move:
                MoveControlsView(transformationManager: transformationManager)
            case .rotate:
                RotateControlsView(transformationManager: transformationManager)
            case .scale:
                ScaleControlsView(transformationManager: transformationManager)
            case .flipHorizontal, .flipVertical:
                FlipControlsView(transformationManager: transformationManager)
            case .skew:
                SkewControlsView(transformationManager: transformationManager)
            case .perspective:
                PerspectiveControlsView(transformationManager: transformationManager)
            }
        }
    }
}

// MARK: - Individual Control Views

struct MoveControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    @State private var moveX: Double = 0
    @State private var moveY: Double = 0
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Move")
                .font(.headline)
                .foregroundColor(.primary)
            
            HStack {
                VStack {
                    Text("X: \(Int(moveX))")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Slider(value: $moveX, in: -200...200)
                        .accentColor(.red)
                }
                
                VStack {
                    Text("Y: \(Int(moveY))")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Slider(value: $moveY, in: -200...200)
                        .accentColor(.green)
                }
            }
        }
    }
}

struct RotateControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Rotate")
                .font(.headline)
                .foregroundColor(.primary)
            
            HStack {
                Text("Angle: \(Int(transformationManager.rotationAngle))°")
                    .font(.subheadline)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Text("\(Int(transformationManager.rotationAngle))°")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Slider(value: $transformationManager.rotationAngle, in: -180...180)
                .accentColor(.blue)
            
            // Quick rotation buttons
            HStack(spacing: 10) {
                Button("90°") {
                    transformationManager.rotationAngle = 90
                }
                .buttonStyle(.bordered)
                
                Button("180°") {
                    transformationManager.rotationAngle = 180
                }
                .buttonStyle(.bordered)
                
                Button("-90°") {
                    transformationManager.rotationAngle = -90
                }
                .buttonStyle(.bordered)
            }
        }
    }
}

struct ScaleControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    @State private var maintainAspectRatio = true
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Scale")
                .font(.headline)
                .foregroundColor(.primary)
            
            Toggle("Maintain Aspect Ratio", isOn: $maintainAspectRatio)
                .toggleStyle(SwitchToggleStyle(tint: .blue))
            
            VStack {
                HStack {
                    Text("X: \(Int(transformationManager.scaleX * 100))%")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("\(Int(transformationManager.scaleX * 100))%")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $transformationManager.scaleX, in: 0.1...3.0)
                    .accentColor(.red)
                    .onChange(of: transformationManager.scaleX) { newValue in
                        if maintainAspectRatio {
                            transformationManager.scaleY = newValue
                        }
                    }
            }
            
            if !maintainAspectRatio {
                VStack {
                    HStack {
                        Text("Y: \(Int(transformationManager.scaleY * 100))%")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Text("\(Int(transformationManager.scaleY * 100))%")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    Slider(value: $transformationManager.scaleY, in: 0.1...3.0)
                        .accentColor(.green)
                }
            }
            
            // Quick scale buttons
            HStack(spacing: 10) {
                Button("50%") {
                    transformationManager.scaleX = 0.5
                    transformationManager.scaleY = 0.5
                }
                .buttonStyle(.bordered)
                
                Button("100%") {
                    transformationManager.scaleX = 1.0
                    transformationManager.scaleY = 1.0
                }
                .buttonStyle(.bordered)
                
                Button("200%") {
                    transformationManager.scaleX = 2.0
                    transformationManager.scaleY = 2.0
                }
                .buttonStyle(.bordered)
            }
        }
    }
}

struct FlipControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Flip")
                .font(.headline)
                .foregroundColor(.primary)
            
            HStack(spacing: 20) {
                Button(action: {
                    transformationManager.currentTool = .flipHorizontal
                    transformationManager.applyTransformation()
                }) {
                    VStack {
                        Image(systemName: "arrow.left.and.right")
                            .font(.system(size: 30))
                            .foregroundColor(.white)
                        
                        Text("Horizontal")
                            .font(.caption)
                            .foregroundColor(.white)
                    }
                    .padding()
                    .background(Color.blue)
                    .cornerRadius(10)
                }
                
                Button(action: {
                    transformationManager.currentTool = .flipVertical
                    transformationManager.applyTransformation()
                }) {
                    VStack {
                        Image(systemName: "arrow.up.and.down")
                            .font(.system(size: 30))
                            .foregroundColor(.white)
                        
                        Text("Vertical")
                            .font(.caption)
                            .foregroundColor(.white)
                    }
                    .padding()
                    .background(Color.green)
                    .cornerRadius(10)
                }
            }
        }
    }
}

struct SkewControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Skew")
                .font(.headline)
                .foregroundColor(.primary)
            
            VStack {
                HStack {
                    Text("X: \(Int(transformationManager.skewX))°")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("\(Int(transformationManager.skewX))°")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $transformationManager.skewX, in: -45...45)
                    .accentColor(.red)
            }
            
            VStack {
                HStack {
                    Text("Y: \(Int(transformationManager.skewY))°")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("\(Int(transformationManager.skewY))°")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $transformationManager.skewY, in: -45...45)
                    .accentColor(.green)
            }
        }
    }
}

struct PerspectiveControlsView: View {
    @ObservedObject var transformationManager: TransformationManager
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Perspective")
                .font(.headline)
                .foregroundColor(.primary)
            
            VStack {
                HStack {
                    Text("X: \(Int(transformationManager.perspectiveX))")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("\(Int(transformationManager.perspectiveX))")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $transformationManager.perspectiveX, in: -100...100)
                    .accentColor(.red)
            }
            
            VStack {
                HStack {
                    Text("Y: \(Int(transformationManager.perspectiveY))")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("\(Int(transformationManager.perspectiveY))")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $transformationManager.perspectiveY, in: -100...100)
                    .accentColor(.green)
            }
        }
    }
}

// MARK: - Transformation Preview View

struct TransformationPreviewView: View {
    @ObservedObject var transformationManager: TransformationManager
    let layer: DrawingLayer
    
    var body: some View {
        ZStack {
            // Original layer strokes
            ForEach(layer.strokes, id: \.id) { stroke in
                DrawingStrokeView(stroke: stroke)
            }
            
            // Transformation center point
            Circle()
                .fill(Color.red)
                .frame(width: 8, height: 8)
                .position(transformationManager.transformCenter)
            
            // Transformation handles
            TransformationHandlesView(transformationManager: transformationManager)
        }
    }
}

// MARK: - Transformation Handles View

struct TransformationHandlesView: View {
    @ObservedObject var transformationManager: TransformationManager
    
    var body: some View {
        // Rotation handle
        if transformationManager.currentTool == .rotate {
            Circle()
                .stroke(Color.blue, lineWidth: 2)
                .frame(width: 60, height: 60)
                .position(
                    x: transformationManager.transformCenter.x,
                    y: transformationManager.transformCenter.y - 40
                )
        }
        
        // Scale handles
        if transformationManager.currentTool == .scale {
            ForEach(0..<8, id: \.self) { index in
                Circle()
                    .fill(Color.blue)
                    .frame(width: 8, height: 8)
                    .position(getScaleHandlePosition(for: index))
            }
        }
    }
    
    private func getScaleHandlePosition(for index: Int) -> CGPoint {
        let center = transformationManager.transformCenter
        let radius: CGFloat = 30
        
        let angle = Double(index) * .pi / 4
        return CGPoint(
            x: center.x + radius * cos(angle),
            y: center.y + radius * sin(angle)
        )
    }
}

// MARK: - Transformation History

extension TransformationManager {
    func undoLastTransformation() {
        guard !transformations.isEmpty else { return }
        transformations.removeLast()
        // Re-apply all remaining transformations
        resetValues()
        for transformation in transformations {
            applyTransformationToLayer(transformation)
        }
    }
    
    func clearTransformationHistory() {
        transformations.removeAll()
        resetValues()
    }
    
    var transformationCount: Int {
        return transformations.count
    }
    
    var mostUsedTransformation: TransformationType {
        let typeCounts = Dictionary(grouping: transformations, by: { $0.type })
            .mapValues { $0.count }
        
        return typeCounts.max(by: { $0.value < $1.value })?.key ?? .move
    }
}