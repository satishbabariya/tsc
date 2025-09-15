import SwiftUI
import CoreGraphics

// MARK: - Drawing Tool Types

enum DrawingTool: String, CaseIterable {
    case brush = "brush"
    case pencil = "pencil"
    case eraser = "eraser"
    case marker = "marker"
    case airbrush = "airbrush"
    case highlighter = "highlighter"
    
    var name: String {
        switch self {
        case .brush: return "Brush"
        case .pencil: return "Pencil"
        case .eraser: return "Eraser"
        case .marker: return "Marker"
        case .airbrush: return "Airbrush"
        case .highlighter: return "Highlighter"
        }
    }
    
    var icon: Image {
        switch self {
        case .brush: return Image(systemName: "paintbrush.fill")
        case .pencil: return Image(systemName: "pencil")
        case .eraser: return Image(systemName: "eraser.fill")
        case .marker: return Image(systemName: "highlighter")
        case .airbrush: return Image(systemName: "spray.can.fill")
        case .highlighter: return Image(systemName: "highlighter")
        }
    }
    
    var defaultSize: CGFloat {
        switch self {
        case .brush: return 20
        case .pencil: return 5
        case .eraser: return 15
        case .marker: return 25
        case .airbrush: return 30
        case .highlighter: return 40
        }
    }
    
    var opacity: Double {
        switch self {
        case .brush: return 1.0
        case .pencil: return 1.0
        case .eraser: return 1.0
        case .marker: return 0.8
        case .airbrush: return 0.6
        case .highlighter: return 0.4
        }
    }
}

// MARK: - Brush Settings

struct BrushSettings: ObservableObject {
    @Published var size: CGFloat = 20
    @Published var opacity: Double = 1.0
    @Published var hardness: Double = 0.5
    @Published var spacing: Double = 0.1
    @Published var flow: Double = 1.0
    @Published var smoothing: Double = 0.5
    
    var maxSize: CGFloat { 100 }
    var minSize: CGFloat { 1 }
}

// MARK: - Drawing Stroke

struct DrawingStroke: Identifiable, Codable {
    let id = UUID()
    let tool: DrawingTool
    let color: Color
    let size: CGFloat
    let opacity: Double
    let hardness: Double
    let points: [CGPoint]
    let timestamp: Date
    
    init(tool: DrawingTool, color: Color, size: CGFloat, opacity: Double, hardness: Double, points: [CGPoint]) {
        self.tool = tool
        self.color = color
        self.size = size
        self.opacity = opacity
        self.hardness = hardness
        self.points = points
        self.timestamp = Date()
    }
}

// MARK: - Drawing Manager

class DrawingManager: ObservableObject {
    @Published var currentTool: DrawingTool = .brush
    @Published var brushSettings = BrushSettings()
    @Published var strokes: [DrawingStroke] = []
    @Published var isDrawing = false
    @Published var currentStroke: DrawingStroke?
    @Published var selectedColor: Color = .black
    @Published var showToolSettings = false
    
    private var strokePoints: [CGPoint] = []
    
    func startStroke(at point: CGPoint) {
        isDrawing = true
        strokePoints = [point]
        
        currentStroke = DrawingStroke(
            tool: currentTool,
            color: selectedColor,
            size: brushSettings.size,
            opacity: brushSettings.opacity,
            hardness: brushSettings.hardness,
            points: strokePoints
        )
    }
    
    func addPoint(_ point: CGPoint) {
        guard isDrawing else { return }
        strokePoints.append(point)
        
        currentStroke = DrawingStroke(
            tool: currentTool,
            color: selectedColor,
            size: brushSettings.size,
            opacity: brushSettings.opacity,
            hardness: brushSettings.hardness,
            points: strokePoints
        )
    }
    
    func endStroke() {
        guard isDrawing, let stroke = currentStroke else { return }
        
        strokes.append(stroke)
        isDrawing = false
        strokePoints = []
        currentStroke = nil
        
        // Record drawing action
        progressTracker.recordAction(.drawStroke)
    }
    
    func clearCanvas() {
        strokes.removeAll()
        currentStroke = nil
        strokePoints = []
        isDrawing = false
    }
    
    func undoLastStroke() {
        if !strokes.isEmpty {
            strokes.removeLast()
        }
    }
    
    func setTool(_ tool: DrawingTool) {
        currentTool = tool
        brushSettings.size = tool.defaultSize
        brushSettings.opacity = tool.opacity
    }
}

// MARK: - Drawing Tool Palette

struct DrawingToolPalette: View {
    @ObservedObject var drawingManager: DrawingManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🖌️ Drawing Tools")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        drawingManager.showToolSettings.toggle()
                    }
                }) {
                    Image(systemName: drawingManager.showToolSettings ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            // Tool selector
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 15) {
                    ForEach(DrawingTool.allCases, id: \.self) { tool in
                        Button(action: {
                            drawingManager.setTool(tool)
                            SafeHapticFeedback.shared.lightImpact()
                        }) {
                            VStack {
                                tool.icon
                                    .font(.system(size: 25))
                                    .foregroundColor(drawingManager.currentTool == tool ? .white : .primary)
                                    .frame(width: 50, height: 50)
                                    .background(
                                        Circle()
                                            .fill(drawingManager.currentTool == tool ? Color.blue : Color.gray.opacity(0.2))
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
            
            if drawingManager.showToolSettings {
                // Tool settings
                VStack(spacing: 15) {
                    // Size control
                    VStack {
                        HStack {
                            Text("Size: \(Int(drawingManager.brushSettings.size))")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(drawingManager.brushSettings.size))px")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(
                            value: Binding(
                                get: { Double(drawingManager.brushSettings.size) },
                                set: { drawingManager.brushSettings.size = CGFloat($0) }
                            ),
                            in: Double(drawingManager.brushSettings.minSize)...Double(drawingManager.brushSettings.maxSize)
                        )
                        .accentColor(.blue)
                    }
                    
                    // Opacity control
                    VStack {
                        HStack {
                            Text("Opacity: \(Int(drawingManager.brushSettings.opacity * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(drawingManager.brushSettings.opacity * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $drawingManager.brushSettings.opacity, in: 0.1...1.0)
                            .accentColor(.green)
                    }
                    
                    // Hardness control (for brush tools)
                    if drawingManager.currentTool == .brush || drawingManager.currentTool == .airbrush {
                        VStack {
                            HStack {
                                Text("Hardness: \(Int(drawingManager.brushSettings.hardness * 100))%")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Spacer()
                                
                                Text("\(Int(drawingManager.brushSettings.hardness * 100))%")
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }
                            
                            Slider(value: $drawingManager.brushSettings.hardness, in: 0.0...1.0)
                                .accentColor(.orange)
                        }
                    }
                    
                    // Flow control (for airbrush)
                    if drawingManager.currentTool == .airbrush {
                        VStack {
                            HStack {
                                Text("Flow: \(Int(drawingManager.brushSettings.flow * 100))%")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Spacer()
                                
                                Text("\(Int(drawingManager.brushSettings.flow * 100))%")
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }
                            
                            Slider(value: $drawingManager.brushSettings.flow, in: 0.1...1.0)
                                .accentColor(.purple)
                        }
                    }
                    
                    // Smoothing control
                    VStack {
                        HStack {
                            Text("Smoothing: \(Int(drawingManager.brushSettings.smoothing * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(drawingManager.brushSettings.smoothing * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $drawingManager.brushSettings.smoothing, in: 0.0...1.0)
                            .accentColor(.cyan)
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
                colors: [Color.blue.opacity(0.05), Color.green.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Drawing Canvas View

struct DrawingCanvasView: View {
    @ObservedObject var drawingManager: DrawingManager
    @ObservedObject var errorHandler: ErrorHandler
    @State private var canvasSize: CGSize = CGSize(width: 400, height: 400)
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Canvas background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Drawing area
                    ZStack {
                        // Canvas
                        Rectangle()
                            .fill(Color.white)
                            .frame(width: canvasSize.width, height: canvasSize.height)
                            .border(Color.gray.opacity(0.3), width: 1)
                        
                        // Drawing strokes
                        ForEach(drawingManager.strokes) { stroke in
                            DrawingStrokeView(stroke: stroke)
                        }
                        
                        // Current stroke being drawn
                        if let currentStroke = drawingManager.currentStroke {
                            DrawingStrokeView(stroke: currentStroke)
                        }
                        
                        // Drawing cursor
                        if drawingManager.isDrawing {
                            Circle()
                                .fill(drawingManager.selectedColor.opacity(0.3))
                                .frame(width: drawingManager.brushSettings.size, height: drawingManager.brushSettings.size)
                                .overlay(
                                    Circle()
                                        .stroke(drawingManager.selectedColor, lineWidth: 2)
                                )
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    .gesture(
                        DragGesture(minimumDistance: 0)
                            .onChanged { value in
                                let point = value.location
                                
                                if !drawingManager.isDrawing {
                                    drawingManager.startStroke(at: point)
                                } else {
                                    drawingManager.addPoint(point)
                                }
                            }
                            .onEnded { _ in
                                drawingManager.endStroke()
                            }
                    )
                    
                    // Action buttons
                    HStack(spacing: 20) {
                        // Undo button
                        Button(action: {
                            drawingManager.undoLastStroke()
                            SafeHapticFeedback.shared.undo()
                        }) {
                            HStack {
                                Image(systemName: "arrow.uturn.backward")
                                Text("Undo")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 15)
                            .padding(.vertical, 8)
                            .background(Color.orange)
                            .cornerRadius(15)
                        }
                        .disabled(drawingManager.strokes.isEmpty)
                        
                        // Clear button
                        Button(action: {
                            drawingManager.clearCanvas()
                            SafeHapticFeedback.shared.clear()
                        }) {
                            HStack {
                                Image(systemName: "trash")
                                Text("Clear")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 15)
                            .padding(.vertical, 8)
                            .background(Color.red)
                            .cornerRadius(15)
                        }
                        
                        Spacer()
                        
                        // Canvas size info
                        Text("\(Int(canvasSize.width)) × \(Int(canvasSize.height))")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    .padding(.horizontal)
                    .padding(.bottom, 10)
                }
            }
        }
    }
}

// MARK: - Drawing Stroke View

struct DrawingStrokeView: View {
    let stroke: DrawingStroke
    
    var body: some View {
        Path { path in
            guard !stroke.points.isEmpty else { return }
            
            path.move(to: stroke.points[0])
            
            for i in 1..<stroke.points.count {
                path.addLine(to: stroke.points[i])
            }
        }
        .stroke(
            stroke.color,
            style: StrokeStyle(
                lineWidth: stroke.size,
                lineCap: .round,
                lineJoin: .round
            )
        )
        .opacity(stroke.opacity)
    }
}

// MARK: - Advanced Brush Engine

class BrushEngine {
    static func createBrushPath(points: [CGPoint], size: CGFloat, hardness: Double) -> Path {
        var path = Path()
        
        guard points.count >= 2 else {
            if let point = points.first {
                path.addEllipse(in: CGRect(
                    x: point.x - size/2,
                    y: point.y - size/2,
                    width: size,
                    height: size
                ))
            }
            return path
        }
        
        // Create smooth path with pressure simulation
        for i in 0..<points.count - 1 {
            let currentPoint = points[i]
            let nextPoint = points[i + 1]
            
            // Calculate distance for pressure variation
            let distance = sqrt(pow(nextPoint.x - currentPoint.x, 2) + pow(nextPoint.y - currentPoint.y, 2))
            let pressure = max(0.5, min(1.0, 1.0 - distance / 50.0))
            
            let adjustedSize = size * CGFloat(pressure)
            
            // Create elliptical brush stroke
            let angle = atan2(nextPoint.y - currentPoint.y, nextPoint.x - currentPoint.x)
            let ellipseRect = CGRect(
                x: currentPoint.x - adjustedSize/2,
                y: currentPoint.y - adjustedSize/2,
                width: adjustedSize,
                height: adjustedSize * CGFloat(hardness)
            )
            
            path.addEllipse(in: ellipseRect)
        }
        
        return path
    }
    
    static func createAirbrushEffect(points: [CGPoint], size: CGFloat, flow: Double) -> Path {
        var path = Path()
        
        for point in points {
            // Create multiple small circles for airbrush effect
            let numDots = Int(flow * 10)
            for _ in 0..<numDots {
                let randomOffset = CGPoint(
                    x: CGFloat.random(in: -size/2...size/2),
                    y: CGFloat.random(in: -size/2...size/2)
                )
                
                let dotSize = CGFloat.random(in: 1...size/4)
                let dotPoint = CGPoint(
                    x: point.x + randomOffset.x,
                    y: point.y + randomOffset.y
                )
                
                path.addEllipse(in: CGRect(
                    x: dotPoint.x - dotSize/2,
                    y: dotPoint.y - dotSize/2,
                    width: dotSize,
                    height: dotSize
                ))
            }
        }
        
        return path
    }
}

// MARK: - Drawing Statistics

extension DrawingManager {
    var totalStrokes: Int {
        return strokes.count
    }
    
    var totalDrawingTime: TimeInterval {
        guard !strokes.isEmpty else { return 0 }
        let firstStroke = strokes.first!.timestamp
        let lastStroke = strokes.last!.timestamp
        return lastStroke.timeIntervalSince(firstStroke)
    }
    
    var mostUsedTool: DrawingTool {
        let toolCounts = Dictionary(grouping: strokes, by: { $0.tool })
            .mapValues { $0.count }
        
        return toolCounts.max(by: { $0.value < $1.value })?.key ?? .brush
    }
    
    var averageStrokeLength: Double {
        guard !strokes.isEmpty else { return 0 }
        
        let totalLength = strokes.reduce(0) { total, stroke in
            var length: Double = 0
            for i in 0..<stroke.points.count - 1 {
                let p1 = stroke.points[i]
                let p2 = stroke.points[i + 1]
                length += sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2))
            }
            return total + length
        }
        
        return totalLength / Double(strokes.count)
    }
}

// MARK: - Drawing Presets

struct DrawingPresets {
    static let realisticBrush = BrushSettings()
    static let sketchPencil = BrushSettings()
    static let markerPen = BrushSettings()
    static let airbrushSpray = BrushSettings()
    
    static func loadPreset(_ preset: DrawingPreset) -> BrushSettings {
        switch preset {
        case .realisticBrush:
            let settings = BrushSettings()
            settings.size = 15
            settings.opacity = 0.8
            settings.hardness = 0.7
            settings.smoothing = 0.3
            return settings
        case .sketchPencil:
            let settings = BrushSettings()
            settings.size = 3
            settings.opacity = 1.0
            settings.hardness = 1.0
            settings.smoothing = 0.1
            return settings
        case .markerPen:
            let settings = BrushSettings()
            settings.size = 25
            settings.opacity = 0.9
            settings.hardness = 0.2
            settings.smoothing = 0.5
            return settings
        case .airbrushSpray:
            let settings = BrushSettings()
            settings.size = 30
            settings.opacity = 0.6
            settings.hardness = 0.1
            settings.flow = 0.7
            settings.smoothing = 0.8
            return settings
        }
    }
}

enum DrawingPreset: String, CaseIterable {
    case realisticBrush = "Realistic Brush"
    case sketchPencil = "Sketch Pencil"
    case markerPen = "Marker Pen"
    case airbrushSpray = "Airbrush Spray"
}