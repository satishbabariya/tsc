import SwiftUI
import CoreGraphics

// MARK: - Blending Modes

enum BlendingMode: String, CaseIterable {
    case normal = "Normal"
    case multiply = "Multiply"
    case screen = "Screen"
    case overlay = "Overlay"
    case softLight = "Soft Light"
    case hardLight = "Hard Light"
    case colorDodge = "Color Dodge"
    case colorBurn = "Color Burn"
    case darken = "Darken"
    case lighten = "Lighten"
    case difference = "Difference"
    case exclusion = "Exclusion"
    case hue = "Hue"
    case saturation = "Saturation"
    case color = "Color"
    case luminosity = "Luminosity"
    
    var description: String {
        switch self {
        case .normal: return "Normal blending"
        case .multiply: return "Darkens the image"
        case .screen: return "Lightens the image"
        case .overlay: return "Combines multiply and screen"
        case .softLight: return "Soft light effect"
        case .hardLight: return "Hard light effect"
        case .colorDodge: return "Brightens base colors"
        case .colorBurn: return "Darkens base colors"
        case .darken: return "Keeps darker pixels"
        case .lighten: return "Keeps lighter pixels"
        case .difference: return "Subtracts colors"
        case .exclusion: return "Similar to difference"
        case .hue: return "Preserves hue only"
        case .saturation: return "Preserves saturation only"
        case .color: return "Preserves color only"
        case .luminosity: return "Preserves luminosity only"
        }
    }
}

// MARK: - Layer Model

class DrawingLayer: ObservableObject, Identifiable {
    let id = UUID()
    @Published var name: String
    @Published var isVisible: Bool = true
    @Published var opacity: Double = 1.0
    @Published var blendingMode: BlendingMode = .normal
    @Published var strokes: [DrawingStroke] = []
    @Published var isLocked: Bool = false
    @Published var thumbnail: UIImage?
    
    init(name: String) {
        self.name = name
        generateThumbnail()
    }
    
    func addStroke(_ stroke: DrawingStroke) {
        strokes.append(stroke)
        generateThumbnail()
    }
    
    func removeStroke(_ stroke: DrawingStroke) {
        strokes.removeAll { $0.id == stroke.id }
        generateThumbnail()
    }
    
    func clearLayer() {
        strokes.removeAll()
        generateThumbnail()
    }
    
    func duplicate() -> DrawingLayer {
        let newLayer = DrawingLayer(name: "\(name) Copy")
        newLayer.strokes = strokes
        newLayer.opacity = opacity
        newLayer.blendingMode = blendingMode
        newLayer.isVisible = isVisible
        return newLayer
    }
    
    private func generateThumbnail() {
        // Generate thumbnail for layer preview
        let size = CGSize(width: 100, height: 100)
        let renderer = UIGraphicsImageRenderer(size: size)
        
        let image = renderer.image { context in
            context.cgContext.setFillColor(UIColor.clear.cgColor)
            context.cgContext.fill(CGRect(origin: .zero, size: size))
            
            // Draw strokes
            for stroke in strokes {
                let path = createPath(from: stroke.points)
                context.cgContext.setStrokeColor(UIColor(stroke.color).cgColor)
                context.cgContext.setLineWidth(stroke.size)
                context.cgContext.setAlpha(CGFloat(stroke.opacity))
                context.cgContext.addPath(path.cgPath)
                context.cgContext.strokePath()
            }
        }
        
        thumbnail = image
    }
    
    private func createPath(from points: [CGPoint]) -> UIBezierPath {
        let path = UIBezierPath()
        guard !points.isEmpty else { return path }
        
        path.move(to: points[0])
        for i in 1..<points.count {
            path.addLine(to: points[i])
        }
        
        return path
    }
}

// MARK: - Layer Manager

class LayerManager: ObservableObject {
    @Published var layers: [DrawingLayer] = []
    @Published var activeLayerIndex: Int = 0
    @Published var showLayerPanel = false
    
    init() {
        // Create default background layer
        let backgroundLayer = DrawingLayer(name: "Background")
        layers.append(backgroundLayer)
    }
    
    var activeLayer: DrawingLayer? {
        guard activeLayerIndex < layers.count else { return nil }
        return layers[activeLayerIndex]
    }
    
    func addLayer(name: String = "New Layer") -> DrawingLayer {
        let newLayer = DrawingLayer(name: name)
        layers.append(newLayer)
        activeLayerIndex = layers.count - 1
        return newLayer
    }
    
    func removeLayer(at index: Int) {
        guard index < layers.count && layers.count > 1 else { return }
        layers.remove(at: index)
        
        if activeLayerIndex >= layers.count {
            activeLayerIndex = layers.count - 1
        }
    }
    
    func moveLayer(from source: IndexSet, to destination: Int) {
        layers.move(fromOffsets: source, toOffset: destination)
        
        // Update active layer index
        if let sourceIndex = source.first {
            if sourceIndex < activeLayerIndex {
                activeLayerIndex -= 1
            } else if sourceIndex == activeLayerIndex {
                activeLayerIndex = destination
            }
        }
    }
    
    func duplicateLayer(at index: Int) {
        guard index < layers.count else { return }
        let duplicatedLayer = layers[index].duplicate()
        layers.insert(duplicatedLayer, at: index + 1)
        activeLayerIndex = index + 1
    }
    
    func mergeDownLayer(at index: Int) {
        guard index > 0 && index < layers.count else { return }
        
        let currentLayer = layers[index]
        let layerBelow = layers[index - 1]
        
        // Merge strokes from current layer to layer below
        layerBelow.strokes.append(contentsOf: currentLayer.strokes)
        layerBelow.generateThumbnail()
        
        // Remove current layer
        layers.remove(at: index)
        
        if activeLayerIndex >= index {
            activeLayerIndex -= 1
        }
    }
    
    func flattenLayers() {
        guard layers.count > 1 else { return }
        
        let backgroundLayer = layers[0]
        
        // Merge all strokes into background layer
        for i in 1..<layers.count {
            backgroundLayer.strokes.append(contentsOf: layers[i].strokes)
        }
        
        // Remove all layers except background
        layers = [backgroundLayer]
        activeLayerIndex = 0
    }
    
    func addStrokeToActiveLayer(_ stroke: DrawingStroke) {
        activeLayer?.addStroke(stroke)
    }
}

// MARK: - Layer Panel View

struct LayerPanelView: View {
    @ObservedObject var layerManager: LayerManager
    @ObservedObject var errorHandler: ErrorHandler
    @State private var showAddLayerDialog = false
    @State private var showRenameDialog = false
    @State private var layerToRename: DrawingLayer?
    @State private var newLayerName = ""
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("📚 Layers")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        layerManager.showLayerPanel.toggle()
                    }
                }) {
                    Image(systemName: layerManager.showLayerPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if layerManager.showLayerPanel {
                VStack(spacing: 15) {
                    // Layer list
                    ScrollView {
                        LazyVStack(spacing: 8) {
                            ForEach(Array(layerManager.layers.enumerated()), id: \.element.id) { index, layer in
                                LayerRowView(
                                    layer: layer,
                                    isActive: index == layerManager.activeLayerIndex,
                                    layerManager: layerManager,
                                    index: index
                                )
                            }
                        }
                    }
                    .frame(maxHeight: 300)
                    
                    // Layer controls
                    HStack(spacing: 15) {
                        // Add layer button
                        Button(action: {
                            showAddLayerDialog = true
                        }) {
                            Image(systemName: "plus")
                                .foregroundColor(.white)
                                .frame(width: 30, height: 30)
                                .background(Color.green)
                                .cornerRadius(15)
                        }
                        
                        // Duplicate layer button
                        Button(action: {
                            layerManager.duplicateLayer(at: layerManager.activeLayerIndex)
                        }) {
                            Image(systemName: "doc.on.doc")
                                .foregroundColor(.white)
                                .frame(width: 30, height: 30)
                                .background(Color.blue)
                                .cornerRadius(15)
                        }
                        .disabled(layerManager.layers.isEmpty)
                        
                        // Merge down button
                        Button(action: {
                            layerManager.mergeDownLayer(at: layerManager.activeLayerIndex)
                        }) {
                            Image(systemName: "arrow.down.circle")
                                .foregroundColor(.white)
                                .frame(width: 30, height: 30)
                                .background(Color.orange)
                                .cornerRadius(15)
                        }
                        .disabled(layerManager.activeLayerIndex == 0 || layerManager.layers.count <= 1)
                        
                        // Delete layer button
                        Button(action: {
                            layerManager.removeLayer(at: layerManager.activeLayerIndex)
                        }) {
                            Image(systemName: "trash")
                                .foregroundColor(.white)
                                .frame(width: 30, height: 30)
                                .background(Color.red)
                                .cornerRadius(15)
                        }
                        .disabled(layerManager.layers.count <= 1)
                        
                        Spacer()
                        
                        // Flatten layers button
                        Button(action: {
                            layerManager.flattenLayers()
                        }) {
                            Text("Flatten")
                                .font(.caption)
                                .foregroundColor(.white)
                                .padding(.horizontal, 10)
                                .padding(.vertical, 5)
                                .background(Color.purple)
                                .cornerRadius(10)
                        }
                        .disabled(layerManager.layers.count <= 1)
                    }
                    .padding(.horizontal)
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
                colors: [Color.purple.opacity(0.05), Color.blue.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
        .sheet(isPresented: $showAddLayerDialog) {
            AddLayerDialogView(layerManager: layerManager)
        }
        .alert("Rename Layer", isPresented: $showRenameDialog) {
            TextField("Layer name", text: $newLayerName)
            Button("Cancel", role: .cancel) { }
            Button("Rename") {
                layerToRename?.name = newLayerName
            }
        }
    }
}

// MARK: - Layer Row View

struct LayerRowView: View {
    @ObservedObject var layer: DrawingLayer
    let isActive: Bool
    @ObservedObject var layerManager: LayerManager
    let index: Int
    @State private var showLayerSettings = false
    
    var body: some View {
        HStack(spacing: 10) {
            // Visibility toggle
            Button(action: {
                layer.isVisible.toggle()
            }) {
                Image(systemName: layer.isVisible ? "eye.fill" : "eye.slash.fill")
                    .foregroundColor(layer.isVisible ? .blue : .gray)
            }
            
            // Layer thumbnail
            if let thumbnail = layer.thumbnail {
                Image(uiImage: thumbnail)
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 40, height: 40)
                    .cornerRadius(5)
                    .overlay(
                        RoundedRectangle(cornerRadius: 5)
                            .stroke(isActive ? Color.blue : Color.clear, lineWidth: 2)
                    )
            } else {
                RoundedRectangle(cornerRadius: 5)
                    .fill(Color.gray.opacity(0.3))
                    .frame(width: 40, height: 40)
                    .overlay(
                        RoundedRectangle(cornerRadius: 5)
                            .stroke(isActive ? Color.blue : Color.clear, lineWidth: 2)
                    )
            }
            
            // Layer name
            Button(action: {
                layerManager.activeLayerIndex = index
            }) {
                Text(layer.name)
                    .font(.subheadline)
                    .foregroundColor(isActive ? .blue : .primary)
                    .fontWeight(isActive ? .bold : .regular)
            }
            .buttonStyle(PlainButtonStyle())
            
            Spacer()
            
            // Layer settings button
            Button(action: {
                showLayerSettings.toggle()
            }) {
                Image(systemName: "slider.horizontal.3")
                    .foregroundColor(.secondary)
            }
            
            // Lock toggle
            Button(action: {
                layer.isLocked.toggle()
            }) {
                Image(systemName: layer.isLocked ? "lock.fill" : "lock.open.fill")
                    .foregroundColor(layer.isLocked ? .orange : .gray)
            }
        }
        .padding(.horizontal, 10)
        .padding(.vertical, 8)
        .background(
            RoundedRectangle(cornerRadius: 8)
                .fill(isActive ? Color.blue.opacity(0.1) : Color.clear)
        )
        .overlay(
            RoundedRectangle(cornerRadius: 8)
                .stroke(isActive ? Color.blue : Color.clear, lineWidth: 1)
        )
        .sheet(isPresented: $showLayerSettings) {
            LayerSettingsView(layer: layer)
        }
    }
}

// MARK: - Layer Settings View

struct LayerSettingsView: View {
    @ObservedObject var layer: DrawingLayer
    @Environment(\.dismiss) private var dismiss
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Layer Settings")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // Layer name
                    VStack(alignment: .leading) {
                        Text("Layer Name")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        TextField("Layer name", text: $layer.name)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    // Opacity
                    VStack(alignment: .leading) {
                        HStack {
                            Text("Opacity")
                                .font(.headline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(layer.opacity * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $layer.opacity, in: 0.0...1.0)
                            .accentColor(.blue)
                    }
                    
                    // Blending mode
                    VStack(alignment: .leading) {
                        Text("Blending Mode")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Picker("Blending Mode", selection: $layer.blendingMode) {
                            ForEach(BlendingMode.allCases, id: \.self) { mode in
                                Text(mode.rawValue).tag(mode)
                            }
                        }
                        .pickerStyle(MenuPickerStyle())
                    }
                    
                    // Layer info
                    VStack(alignment: .leading) {
                        Text("Layer Info")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        VStack(alignment: .leading, spacing: 5) {
                            Text("Strokes: \(layer.strokes.count)")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            Text("Visible: \(layer.isVisible ? "Yes" : "No")")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            Text("Locked: \(layer.isLocked ? "Yes" : "No")")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                        }
                    }
                }
                
                Spacer()
                
                Button("Done") {
                    dismiss()
                }
                .buttonStyle(.borderedProminent)
            }
            .padding()
            .navigationBarHidden(true)
        }
    }
}

// MARK: - Add Layer Dialog

struct AddLayerDialogView: View {
    @ObservedObject var layerManager: LayerManager
    @Environment(\.dismiss) private var dismiss
    @State private var layerName = "New Layer"
    @State private var layerType: LayerType = .normal
    
    enum LayerType: String, CaseIterable {
        case normal = "Normal"
        case adjustment = "Adjustment"
        case text = "Text"
        case shape = "Shape"
    }
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Add New Layer")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // Layer name
                    VStack(alignment: .leading) {
                        Text("Layer Name")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        TextField("Enter layer name", text: $layerName)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    // Layer type
                    VStack(alignment: .leading) {
                        Text("Layer Type")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Picker("Layer Type", selection: $layerType) {
                            ForEach(LayerType.allCases, id: \.self) { type in
                                Text(type.rawValue).tag(type)
                            }
                        }
                        .pickerStyle(SegmentedPickerStyle())
                    }
                }
                
                Spacer()
                
                HStack(spacing: 15) {
                    Button("Cancel") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Add Layer") {
                        let newLayer = layerManager.addLayer(name: layerName)
                        dismiss()
                    }
                    .buttonStyle(.borderedProminent)
                }
            }
            .padding()
            .navigationBarHidden(true)
        }
    }
}

// MARK: - Layer Blending Engine

class LayerBlendingEngine {
    static func blendLayers(_ layers: [DrawingLayer]) -> UIImage? {
        guard !layers.isEmpty else { return nil }
        
        let canvasSize = CGSize(width: 400, height: 400)
        let renderer = UIGraphicsImageRenderer(size: canvasSize)
        
        return renderer.image { context in
            // Start with transparent background
            context.cgContext.clear(CGRect(origin: .zero, size: canvasSize))
            
            // Render each visible layer
            for layer in layers where layer.isVisible {
                context.cgContext.saveGState()
                
                // Set opacity
                context.cgContext.setAlpha(CGFloat(layer.opacity))
                
                // Apply blending mode
                applyBlendingMode(layer.blendingMode, to: context.cgContext)
                
                // Draw layer strokes
                for stroke in layer.strokes {
                    drawStroke(stroke, in: context.cgContext)
                }
                
                context.cgContext.restoreGState()
            }
        }
    }
    
    private static func applyBlendingMode(_ mode: BlendingMode, to context: CGContext) {
        switch mode {
        case .normal:
            context.setBlendMode(.normal)
        case .multiply:
            context.setBlendMode(.multiply)
        case .screen:
            context.setBlendMode(.screen)
        case .overlay:
            context.setBlendMode(.overlay)
        case .softLight:
            context.setBlendMode(.softLight)
        case .hardLight:
            context.setBlendMode(.hardLight)
        case .colorDodge:
            context.setBlendMode(.colorDodge)
        case .colorBurn:
            context.setBlendMode(.colorBurn)
        case .darken:
            context.setBlendMode(.darken)
        case .lighten:
            context.setBlendMode(.lighten)
        case .difference:
            context.setBlendMode(.difference)
        case .exclusion:
            context.setBlendMode(.exclusion)
        case .hue:
            context.setBlendMode(.hue)
        case .saturation:
            context.setBlendMode(.saturation)
        case .color:
            context.setBlendMode(.color)
        case .luminosity:
            context.setBlendMode(.luminosity)
        }
    }
    
    private static func drawStroke(_ stroke: DrawingStroke, in context: CGContext) {
        guard !stroke.points.isEmpty else { return }
        
        context.setStrokeColor(UIColor(stroke.color).cgColor)
        context.setLineWidth(stroke.size)
        context.setLineCap(.round)
        context.setLineJoin(.round)
        context.setAlpha(CGFloat(stroke.opacity))
        
        context.move(to: stroke.points[0])
        for i in 1..<stroke.points.count {
            context.addLine(to: stroke.points[i])
        }
        
        context.strokePath()
    }
}