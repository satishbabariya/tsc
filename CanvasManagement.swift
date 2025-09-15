import SwiftUI
import CoreGraphics

// MARK: - Canvas Size Presets

enum CanvasPreset: String, CaseIterable {
    case square = "square"
    case portrait = "portrait"
    case landscape = "landscape"
    case wide = "wide"
    case tall = "tall"
    case custom = "custom"
    
    var name: String {
        switch self {
        case .square: return "Square"
        case .portrait: return "Portrait"
        case .landscape: return "Landscape"
        case .wide: return "Wide"
        case .tall: return "Tall"
        case .custom: return "Custom"
        }
    }
    
    var size: CGSize {
        switch self {
        case .square: return CGSize(width: 400, height: 400)
        case .portrait: return CGSize(width: 300, height: 400)
        case .landscape: return CGSize(width: 400, height: 300)
        case .wide: return CGSize(width: 600, height: 300)
        case .tall: return CGSize(width: 300, height: 600)
        case .custom: return CGSize(width: 400, height: 400)
        }
    }
    
    var icon: Image {
        switch self {
        case .square: return Image(systemName: "square")
        case .portrait: return Image(systemName: "rectangle.portrait")
        case .landscape: return Image(systemName: "rectangle")
        case .wide: return Image(systemName: "rectangle.landscape")
        case .tall: return Image(systemName: "rectangle.portrait")
        case .custom: return Image(systemName: "slider.horizontal.3")
        }
    }
}

// MARK: - Canvas Model

class Canvas: ObservableObject {
    @Published var size: CGSize
    @Published var backgroundColor: Color
    @Published var rotation: Double
    @Published var cropRect: CGRect?
    @Published var zoomLevel: Double
    @Published var panOffset: CGSize
    @Published var gridVisible: Bool
    @Published var gridSize: CGFloat
    @Published var snapToGrid: Bool
    
    init(size: CGSize = CGSize(width: 400, height: 400)) {
        self.size = size
        self.backgroundColor = .white
        self.rotation = 0
        self.cropRect = nil
        self.zoomLevel = 1.0
        self.panOffset = .zero
        self.gridVisible = false
        self.gridSize = 20
        self.snapToGrid = false
    }
    
    func resize(to newSize: CGSize) {
        size = newSize
    }
    
    func rotate(by angle: Double) {
        rotation += angle
        // Normalize rotation to 0-360 degrees
        while rotation >= 360 { rotation -= 360 }
        while rotation < 0 { rotation += 360 }
    }
    
    func setRotation(_ angle: Double) {
        rotation = angle
    }
    
    func crop(to rect: CGRect) {
        cropRect = rect
    }
    
    func clearCrop() {
        cropRect = nil
    }
    
    func zoom(to level: Double) {
        zoomLevel = max(0.1, min(5.0, level))
    }
    
    func pan(by offset: CGSize) {
        panOffset = CGSize(
            width: panOffset.width + offset.width,
            height: panOffset.height + offset.height
        )
    }
    
    func resetView() {
        zoomLevel = 1.0
        panOffset = .zero
        rotation = 0
    }
}

// MARK: - Canvas Manager

class CanvasManager: ObservableObject {
    @Published var canvas: Canvas
    @Published var showCanvasPanel = false
    @Published var selectedPreset: CanvasPreset = .square
    @Published var customWidth: Double = 400
    @Published var customHeight: Double = 400
    @Published var showResizeDialog = false
    @Published var showCropDialog = false
    @Published var showRotateDialog = false
    @Published var cropStartPoint: CGPoint = .zero
    @Published var cropEndPoint: CGPoint = .zero
    @Published var isCropping = false
    
    init() {
        self.canvas = Canvas()
    }
    
    func applyPreset(_ preset: CanvasPreset) {
        selectedPreset = preset
        if preset != .custom {
            canvas.resize(to: preset.size)
        }
    }
    
    func applyCustomSize() {
        canvas.resize(to: CGSize(width: customWidth, height: customHeight))
    }
    
    func startCrop(at point: CGPoint) {
        cropStartPoint = point
        isCropping = true
    }
    
    func updateCrop(to point: CGPoint) {
        cropEndPoint = point
    }
    
    func endCrop() {
        let rect = CGRect(
            x: min(cropStartPoint.x, cropEndPoint.x),
            y: min(cropStartPoint.y, cropEndPoint.y),
            width: abs(cropEndPoint.x - cropStartPoint.x),
            height: abs(cropEndPoint.y - cropStartPoint.y)
        )
        canvas.crop(to: rect)
        isCropping = false
    }
    
    func cancelCrop() {
        isCropping = false
        cropStartPoint = .zero
        cropEndPoint = .zero
    }
    
    func rotateCanvas(by angle: Double) {
        canvas.rotate(by: angle)
    }
    
    func flipCanvasHorizontally() {
        // Implementation would depend on how content is stored
        canvas.rotation += 180
    }
    
    func flipCanvasVertically() {
        // Implementation would depend on how content is stored
        canvas.rotation += 180
    }
}

// MARK: - Canvas Panel View

struct CanvasPanelView: View {
    @ObservedObject var canvasManager: CanvasManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🖼️ Canvas Management")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        canvasManager.showCanvasPanel.toggle()
                    }
                }) {
                    Image(systemName: canvasManager.showCanvasPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if canvasManager.showCanvasPanel {
                VStack(spacing: 15) {
                    // Canvas size presets
                    VStack {
                        Text("Canvas Size")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        ScrollView(.horizontal, showsIndicators: false) {
                            HStack(spacing: 15) {
                                ForEach(CanvasPreset.allCases, id: \.self) { preset in
                                    Button(action: {
                                        canvasManager.applyPreset(preset)
                                        SafeHapticFeedback.shared.lightImpact()
                                    }) {
                                        VStack {
                                            preset.icon
                                                .font(.system(size: 25))
                                                .foregroundColor(canvasManager.selectedPreset == preset ? .white : .primary)
                                                .frame(width: 50, height: 50)
                                                .background(
                                                    RoundedRectangle(cornerRadius: 8)
                                                        .fill(canvasManager.selectedPreset == preset ? Color.blue : Color.gray.opacity(0.2))
                                                )
                                            
                                            Text(preset.name)
                                                .font(.caption)
                                                .foregroundColor(.primary)
                                        }
                                    }
                                    .buttonStyle(PlainButtonStyle())
                                }
                            }
                            .padding(.horizontal)
                        }
                    }
                    
                    // Custom size controls
                    if canvasManager.selectedPreset == .custom {
                        VStack {
                            Text("Custom Size")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            HStack {
                                VStack {
                                    Text("Width")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                    
                                    TextField("Width", value: $canvasManager.customWidth, format: .number)
                                        .textFieldStyle(RoundedBorderTextFieldStyle())
                                        .frame(width: 80)
                                }
                                
                                Text("×")
                                    .font(.title2)
                                    .foregroundColor(.secondary)
                                
                                VStack {
                                    Text("Height")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                    
                                    TextField("Height", value: $canvasManager.customHeight, format: .number)
                                        .textFieldStyle(RoundedBorderTextFieldStyle())
                                        .frame(width: 80)
                                }
                            }
                            
                            Button("Apply Size") {
                                canvasManager.applyCustomSize()
                                SafeHapticFeedback.shared.lightImpact()
                            }
                            .buttonStyle(.borderedProminent)
                        }
                    }
                    
                    // Canvas info
                    VStack {
                        Text("Canvas Info")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        VStack(spacing: 5) {
                            Text("Size: \(Int(canvasManager.canvas.size.width)) × \(Int(canvasManager.canvas.size.height))")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            Text("Rotation: \(Int(canvasManager.canvas.rotation))°")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            Text("Zoom: \(Int(canvasManager.canvas.zoomLevel * 100))%")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                        }
                    }
                    
                    // Canvas tools
                    VStack {
                        Text("Canvas Tools")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack(spacing: 15) {
                            Button("Resize") {
                                canvasManager.showResizeDialog = true
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Crop") {
                                canvasManager.showCropDialog = true
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Rotate") {
                                canvasManager.showRotateDialog = true
                            }
                            .buttonStyle(.bordered)
                        }
                        
                        HStack(spacing: 15) {
                            Button("Flip H") {
                                canvasManager.flipCanvasHorizontally()
                                SafeHapticFeedback.shared.lightImpact()
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Flip V") {
                                canvasManager.flipCanvasVertically()
                                SafeHapticFeedback.shared.lightImpact()
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Reset") {
                                canvasManager.canvas.resetView()
                                SafeHapticFeedback.shared.lightImpact()
                            }
                            .buttonStyle(.bordered)
                        }
                    }
                    
                    // Grid settings
                    VStack {
                        Text("Grid Settings")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack {
                            Toggle("Show Grid", isOn: $canvasManager.canvas.gridVisible)
                                .toggleStyle(SwitchToggleStyle(tint: .blue))
                            
                            Spacer()
                            
                            Toggle("Snap to Grid", isOn: $canvasManager.canvas.snapToGrid)
                                .toggleStyle(SwitchToggleStyle(tint: .green))
                        }
                        
                        if canvasManager.canvas.gridVisible {
                            VStack {
                                HStack {
                                    Text("Grid Size: \(Int(canvasManager.canvas.gridSize))")
                                        .font(.subheadline)
                                        .foregroundColor(.primary)
                                    
                                    Spacer()
                                    
                                    Text("\(Int(canvasManager.canvas.gridSize))px")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }
                                
                                Slider(value: $canvasManager.canvas.gridSize, in: 10...50)
                                    .accentColor(.blue)
                            }
                        }
                    }
                    
                    // Background color
                    VStack {
                        Text("Background Color")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                            ForEach([
                                Color.white, Color.black, Color.red, Color.blue,
                                Color.green, Color.yellow, Color.orange, Color.purple,
                                Color.pink, Color.brown, Color.gray, Color.cyan
                            ], id: \.self) { color in
                                Button(action: {
                                    canvasManager.canvas.backgroundColor = color
                                }) {
                                    Circle()
                                        .fill(color)
                                        .frame(width: 30, height: 30)
                                        .overlay(
                                            Circle()
                                                .stroke(canvasManager.canvas.backgroundColor == color ? Color.black : Color.clear, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
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
                colors: [Color.gray.opacity(0.05), Color.blue.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
        .sheet(isPresented: $canvasManager.showResizeDialog) {
            ResizeDialogView(canvasManager: canvasManager)
        }
        .sheet(isPresented: $canvasManager.showCropDialog) {
            CropDialogView(canvasManager: canvasManager)
        }
        .sheet(isPresented: $canvasManager.showRotateDialog) {
            RotateDialogView(canvasManager: canvasManager)
        }
    }
}

// MARK: - Resize Dialog View

struct ResizeDialogView: View {
    @ObservedObject var canvasManager: CanvasManager
    @Environment(\.dismiss) private var dismiss
    @State private var newWidth: Double = 400
    @State private var newHeight: Double = 400
    @State private var maintainAspectRatio = true
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Resize Canvas")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // Current size
                    VStack {
                        Text("Current Size")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Text("\(Int(canvasManager.canvas.size.width)) × \(Int(canvasManager.canvas.size.height))")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                    }
                    
                    // New size
                    VStack {
                        Text("New Size")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack {
                            VStack {
                                Text("Width")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                TextField("Width", value: $newWidth, format: .number)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                                    .onChange(of: newWidth) { _ in
                                        if maintainAspectRatio {
                                            let ratio = canvasManager.canvas.size.height / canvasManager.canvas.size.width
                                            newHeight = newWidth * ratio
                                        }
                                    }
                            }
                            
                            Text("×")
                                .font(.title2)
                                .foregroundColor(.secondary)
                            
                            VStack {
                                Text("Height")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                TextField("Height", value: $newHeight, format: .number)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                                    .onChange(of: newHeight) { _ in
                                        if maintainAspectRatio {
                                            let ratio = canvasManager.canvas.size.width / canvasManager.canvas.size.height
                                            newWidth = newHeight * ratio
                                        }
                                    }
                            }
                        }
                        
                        Toggle("Maintain Aspect Ratio", isOn: $maintainAspectRatio)
                            .toggleStyle(SwitchToggleStyle(tint: .blue))
                    }
                    
                    // Resize options
                    VStack {
                        Text("Resize Options")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        VStack(alignment: .leading, spacing: 10) {
                            Button("Scale Content") {
                                // Scale all content proportionally
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Crop Content") {
                                // Crop content to fit new size
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Add Padding") {
                                // Add padding around content
                            }
                            .buttonStyle(.bordered)
                        }
                    }
                }
                
                Spacer()
                
                HStack(spacing: 15) {
                    Button("Cancel") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Resize") {
                        canvasManager.canvas.resize(to: CGSize(width: newWidth, height: newHeight))
                        dismiss()
                    }
                    .buttonStyle(.borderedProminent)
                }
            }
            .padding()
            .navigationBarHidden(true)
            .onAppear {
                newWidth = canvasManager.canvas.size.width
                newHeight = canvasManager.canvas.size.height
            }
        }
    }
}

// MARK: - Crop Dialog View

struct CropDialogView: View {
    @ObservedObject var canvasManager: CanvasManager
    @Environment(\.dismiss) private var dismiss
    @State private var cropX: Double = 0
    @State private var cropY: Double = 0
    @State private var cropWidth: Double = 400
    @State private var cropHeight: Double = 400
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Crop Canvas")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // Crop area
                    VStack {
                        Text("Crop Area")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack {
                            VStack {
                                Text("X")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                TextField("X", value: $cropX, format: .number)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                            }
                            
                            VStack {
                                Text("Y")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                TextField("Y", value: $cropY, format: .number)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                            }
                        }
                        
                        HStack {
                            VStack {
                                Text("Width")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                TextField("Width", value: $cropWidth, format: .number)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                            }
                            
                            VStack {
                                Text("Height")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                TextField("Height", value: $cropHeight, format: .number)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                            }
                        }
                    }
                    
                    // Crop preview
                    VStack {
                        Text("Preview")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Rectangle()
                            .fill(Color.gray.opacity(0.3))
                            .frame(width: 200, height: 150)
                            .overlay(
                                Rectangle()
                                    .stroke(Color.blue, lineWidth: 2)
                                    .frame(
                                        width: cropWidth * 200 / canvasManager.canvas.size.width,
                                        height: cropHeight * 150 / canvasManager.canvas.size.height
                                    )
                                    .position(
                                        x: cropX * 200 / canvasManager.canvas.size.width + cropWidth * 100 / canvasManager.canvas.size.width,
                                        y: cropY * 150 / canvasManager.canvas.size.height + cropHeight * 75 / canvasManager.canvas.size.height
                                    )
                            )
                    }
                }
                
                Spacer()
                
                HStack(spacing: 15) {
                    Button("Cancel") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Crop") {
                        let rect = CGRect(x: cropX, y: cropY, width: cropWidth, height: cropHeight)
                        canvasManager.canvas.crop(to: rect)
                        dismiss()
                    }
                    .buttonStyle(.borderedProminent)
                }
            }
            .padding()
            .navigationBarHidden(true)
            .onAppear {
                cropWidth = canvasManager.canvas.size.width
                cropHeight = canvasManager.canvas.size.height
            }
        }
    }
}

// MARK: - Rotate Dialog View

struct RotateDialogView: View {
    @ObservedObject var canvasManager: CanvasManager
    @Environment(\.dismiss) private var dismiss
    @State private var rotationAngle: Double = 0
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Rotate Canvas")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // Current rotation
                    VStack {
                        Text("Current Rotation")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Text("\(Int(canvasManager.canvas.rotation))°")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                    }
                    
                    // Rotation angle
                    VStack {
                        Text("Rotation Angle")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack {
                            Text("Angle: \(Int(rotationAngle))°")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(rotationAngle))°")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $rotationAngle, in: -180...180)
                            .accentColor(.blue)
                    }
                    
                    // Quick rotation buttons
                    VStack {
                        Text("Quick Rotations")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack(spacing: 15) {
                            Button("90°") {
                                rotationAngle = 90
                            }
                            .buttonStyle(.bordered)
                            
                            Button("180°") {
                                rotationAngle = 180
                            }
                            .buttonStyle(.bordered)
                            
                            Button("-90°") {
                                rotationAngle = -90
                            }
                            .buttonStyle(.bordered)
                            
                            Button("Reset") {
                                rotationAngle = 0
                            }
                            .buttonStyle(.bordered)
                        }
                    }
                    
                    // Rotation preview
                    VStack {
                        Text("Preview")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Rectangle()
                            .fill(Color.blue.opacity(0.3))
                            .frame(width: 100, height: 60)
                            .rotationEffect(.degrees(rotationAngle))
                            .overlay(
                                Rectangle()
                                    .stroke(Color.blue, lineWidth: 2)
                                    .frame(width: 100, height: 60)
                                    .rotationEffect(.degrees(rotationAngle))
                            )
                    }
                }
                
                Spacer()
                
                HStack(spacing: 15) {
                    Button("Cancel") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Rotate") {
                        canvasManager.rotateCanvas(by: rotationAngle)
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

// MARK: - Canvas View with Management

struct ManagedCanvasView: View {
    @ObservedObject var canvasManager: CanvasManager
    @ObservedObject var errorHandler: ErrorHandler
    @State private var dragOffset: CGSize = .zero
    @State private var isDragging = false
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Canvas background
                RoundedRectangle(cornerRadius: 20)
                    .fill(canvasManager.canvas.backgroundColor)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Canvas area
                    ZStack {
                        // Grid
                        if canvasManager.canvas.gridVisible {
                            GridView(
                                size: canvasManager.canvas.size,
                                gridSize: canvasManager.canvas.gridSize
                            )
                        }
                        
                        // Canvas content
                        Rectangle()
                            .fill(Color.clear)
                            .frame(width: canvasManager.canvas.size.width, height: canvasManager.canvas.size.height)
                            .border(Color.gray.opacity(0.3), width: 1)
                            .rotationEffect(.degrees(canvasManager.canvas.rotation))
                            .scaleEffect(canvasManager.canvas.zoomLevel)
                            .offset(canvasManager.canvas.panOffset)
                        
                        // Crop overlay
                        if let cropRect = canvasManager.canvas.cropRect {
                            Rectangle()
                                .fill(Color.black.opacity(0.5))
                                .frame(width: canvasManager.canvas.size.width, height: canvasManager.canvas.size.height)
                                .overlay(
                                    Rectangle()
                                        .fill(Color.clear)
                                        .frame(width: cropRect.width, height: cropRect.height)
                                        .position(
                                            x: cropRect.midX,
                                            y: cropRect.midY
                                        )
                                )
                        }
                        
                        // Crop selection (when cropping)
                        if canvasManager.isCropping {
                            Rectangle()
                                .fill(Color.blue.opacity(0.3))
                                .frame(
                                    width: abs(canvasManager.cropEndPoint.x - canvasManager.cropStartPoint.x),
                                    height: abs(canvasManager.cropEndPoint.y - canvasManager.cropStartPoint.y)
                                )
                                .position(
                                    x: (canvasManager.cropStartPoint.x + canvasManager.cropEndPoint.x) / 2,
                                    y: (canvasManager.cropStartPoint.y + canvasManager.cropEndPoint.y) / 2
                                )
                                .overlay(
                                    Rectangle()
                                        .stroke(Color.blue, lineWidth: 2)
                                        .frame(
                                            width: abs(canvasManager.cropEndPoint.x - canvasManager.cropStartPoint.x),
                                            height: abs(canvasManager.cropEndPoint.y - canvasManager.cropStartPoint.y)
                                        )
                                        .position(
                                            x: (canvasManager.cropStartPoint.x + canvasManager.cropEndPoint.x) / 2,
                                            y: (canvasManager.cropStartPoint.y + canvasManager.cropEndPoint.y) / 2
                                        )
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
                                if canvasManager.isCropping {
                                    canvasManager.updateCrop(to: value.location)
                                } else if !isDragging {
                                    isDragging = true
                                    dragOffset = value.translation
                                } else {
                                    dragOffset = value.translation
                                }
                            }
                            .onEnded { value in
                                if canvasManager.isCropping {
                                    canvasManager.endCrop()
                                } else {
                                    canvasManager.canvas.pan(by: value.translation)
                                    dragOffset = .zero
                                    isDragging = false
                                }
                            }
                    )
                    .onTapGesture { location in
                        if canvasManager.isCropping {
                            canvasManager.startCrop(at: location)
                        }
                    }
                    .gesture(
                        MagnificationGesture()
                            .onChanged { value in
                                canvasManager.canvas.zoom(to: value)
                            }
                    )
                    
                    // Canvas info
                    HStack {
                        Text("Canvas: \(Int(canvasManager.canvas.size.width)) × \(Int(canvasManager.canvas.size.height))")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        
                        Spacer()
                        
                        if canvasManager.canvas.rotation != 0 {
                            Text("Rotated: \(Int(canvasManager.canvas.rotation))°")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        if canvasManager.canvas.zoomLevel != 1.0 {
                            Text("Zoom: \(Int(canvasManager.canvas.zoomLevel * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                    }
                    .padding(.horizontal)
                    .padding(.bottom, 10)
                }
            }
        }
    }
}

// MARK: - Grid View

struct GridView: View {
    let size: CGSize
    let gridSize: CGFloat
    
    var body: some View {
        ZStack {
            // Vertical lines
            ForEach(0..<Int(size.width / gridSize), id: \.self) { i in
                Rectangle()
                    .fill(Color.gray.opacity(0.3))
                    .frame(width: 1, height: size.height)
                    .position(x: CGFloat(i) * gridSize, y: size.height / 2)
            }
            
            // Horizontal lines
            ForEach(0..<Int(size.height / gridSize), id: \.self) { i in
                Rectangle()
                    .fill(Color.gray.opacity(0.3))
                    .frame(width: size.width, height: 1)
                    .position(x: size.width / 2, y: CGFloat(i) * gridSize)
            }
        }
    }
}

// MARK: - Canvas Statistics

extension CanvasManager {
    var canvasArea: Double {
        return Double(canvas.size.width * canvas.size.height)
    }
    
    var aspectRatio: Double {
        return Double(canvas.size.width / canvas.size.height)
    }
    
    var isPortrait: Bool {
        return canvas.size.height > canvas.size.width
    }
    
    var isLandscape: Bool {
        return canvas.size.width > canvas.size.height
    }
    
    var isSquare: Bool {
        return abs(canvas.size.width - canvas.size.height) < 1
    }
}