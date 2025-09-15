import SwiftUI

@main
struct ProfessionalColorFillingApp: App {
    @StateObject private var dashboardManager = DashboardManager()
    @StateObject private var progressTracker = ProgressTracker()
    @StateObject private var artworkManager = ArtworkManager()
    @StateObject private var audioManager = AudioManager()
    @StateObject private var colorMixingSystem = ColorMixingSystem()
    @StateObject private var stickerManager = StickerManager()
    @StateObject private var drawingManager = DrawingManager()
    @StateObject private var layerManager = LayerManager()
    @StateObject private var selectionManager = SelectionManager()
    @StateObject private var transformationManager = TransformationManager()
    @StateObject private var filterManager = FilterManager()
    @StateObject private var textToolManager = TextToolManager()
    @StateObject private var gradientPatternManager = GradientPatternManager()
    @StateObject private var advancedColorManager = AdvancedColorManager()
    @StateObject private var canvasManager = CanvasManager()
    @StateObject private var exportManager = ExportManager()
    
    var body: some Scene {
        WindowGroup {
            ProfessionalContentView()
                .environmentObject(dashboardManager)
                .environmentObject(progressTracker)
                .environmentObject(artworkManager)
                .environmentObject(audioManager)
                .environmentObject(colorMixingSystem)
                .environmentObject(stickerManager)
                .environmentObject(drawingManager)
                .environmentObject(layerManager)
                .environmentObject(selectionManager)
                .environmentObject(transformationManager)
                .environmentObject(filterManager)
                .environmentObject(textToolManager)
                .environmentObject(gradientPatternManager)
                .environmentObject(advancedColorManager)
                .environmentObject(canvasManager)
                .environmentObject(exportManager)
                .onAppear {
                    setupProfessionalApp()
                }
        }
    }
    
    private func setupProfessionalApp() {
        // Initialize professional audio system
        EnhancedHapticFeedback.shared.setAudioManager(audioManager)
        
        // Start professional background music
        if audioManager.isMusicEnabled {
            audioManager.playMusic(audioManager.currentMusicTrack)
        }
        
        // Record professional app launch
        progressTracker.recordAction(.sessionStart)
        
        // Initialize professional canvas
        canvasManager.canvas = Canvas(size: CGSize(width: 800, height: 600))
    }
}

struct ProfessionalContentView: View {
    @EnvironmentObject var dashboardManager: DashboardManager
    @EnvironmentObject var progressTracker: ProgressTracker
    @EnvironmentObject var artworkManager: ArtworkManager
    @EnvironmentObject var audioManager: AudioManager
    @EnvironmentObject var colorMixingSystem: ColorMixingSystem
    @EnvironmentObject var stickerManager: StickerManager
    @EnvironmentObject var drawingManager: DrawingManager
    @EnvironmentObject var layerManager: LayerManager
    @EnvironmentObject var selectionManager: SelectionManager
    @EnvironmentObject var transformationManager: TransformationManager
    @EnvironmentObject var filterManager: FilterManager
    @EnvironmentObject var textToolManager: TextToolManager
    @EnvironmentObject var gradientPatternManager: GradientPatternManager
    @EnvironmentObject var advancedColorManager: AdvancedColorManager
    @EnvironmentObject var canvasManager: CanvasManager
    @EnvironmentObject var exportManager: ExportManager
    
    @State private var selectedTemplate: ExtendedColoringTemplate = .butterfly
    @State private var selectedColor: Color = .red
    @State private var animateColors = false
    @StateObject private var errorHandler = ErrorHandler()
    @StateObject private var memoryMonitor = MemoryMonitor()
    @StateObject private var undoRedoManager = UndoRedoManager()
    @State private var currentMode: ProfessionalMode = .drawing
    
    enum ProfessionalMode: String, CaseIterable {
        case drawing = "Drawing"
        case coloring = "Coloring"
        case text = "Text"
        case design = "Design"
        
        var icon: Image {
            switch self {
            case .drawing: return Image(systemName: "paintbrush.fill")
            case .coloring: return Image(systemName: "paintpalette.fill")
            case .text: return Image(systemName: "textformat")
            case .design: return Image(systemName: "square.stack.3d.up")
            }
        }
    }
    
    var body: some View {
        NavigationView {
            ZStack {
                VStack(spacing: 0) {
                    // Professional header with mode selector
                    ProfessionalHeaderView(
                        currentMode: $currentMode,
                        errorHandler: errorHandler
                    )
                    
                    // Professional canvas area
                    ProfessionalCanvasView(
                        currentMode: currentMode,
                        selectedColor: $selectedColor,
                        errorHandler: errorHandler,
                        undoRedoManager: undoRedoManager,
                        stickerManager: stickerManager,
                        drawingManager: drawingManager,
                        layerManager: layerManager,
                        selectionManager: selectionManager,
                        transformationManager: transformationManager,
                        textToolManager: textToolManager,
                        gradientPatternManager: gradientPatternManager,
                        canvasManager: canvasManager
                    )
                    
                    // Professional tool panels
                    ProfessionalToolPanelsView(
                        currentMode: currentMode,
                        selectedColor: $selectedColor,
                        errorHandler: errorHandler,
                        drawingManager: drawingManager,
                        layerManager: layerManager,
                        selectionManager: selectionManager,
                        transformationManager: transformationManager,
                        filterManager: filterManager,
                        textToolManager: textToolManager,
                        gradientPatternManager: gradientPatternManager,
                        advancedColorManager: advancedColorManager,
                        canvasManager: canvasManager,
                        exportManager: exportManager
                    )
                }
                .navigationTitle("🎨 Professional Art Studio")
                .navigationBarTitleDisplayMode(.inline)
                .toolbar {
                    ToolbarItem(placement: .navigationBarLeading) {
                        Button(action: {
                            dashboardManager.showDashboard = true
                        }) {
                            Image(systemName: "person.circle")
                                .foregroundColor(.blue)
                        }
                    }
                    
                    ToolbarItem(placement: .navigationBarTrailing) {
                        Button(action: {
                            exportManager.showExportDialog = true
                        }) {
                            Image(systemName: "square.and.arrow.up")
                                .foregroundColor(.green)
                        }
                    }
                }
                .onAppear {
                    withAnimation(.easeInOut(duration: 2).repeatForever(autoreverses: true)) {
                        animateColors = true
                    }
                    memoryMonitor.startMonitoring()
                }
                .onDisappear {
                    memoryMonitor.stopMonitoring()
                    progressTracker.recordAction(.sessionEnd(0))
                }
                
                // Error alert overlay
                if errorHandler.showErrorAlert {
                    Color.black.opacity(0.3)
                        .ignoresSafeArea()
                        .onTapGesture {
                            errorHandler.clearError()
                        }
                    
                    ErrorAlertView(errorHandler: errorHandler)
                        .transition(.scale.combined(with: .opacity))
                }
                
                // Memory warning overlay
                MemoryWarningView(
                    memoryMonitor: memoryMonitor,
                    errorHandler: errorHandler
                )
                .transition(.scale.combined(with: .opacity))
            }
        }
        .preferredColorScheme(.light)
        .sheet(isPresented: $dashboardManager.showDashboard) {
            ParentTeacherDashboardView(
                dashboardManager: dashboardManager,
                progressTracker: progressTracker,
                artworkManager: artworkManager
            )
        }
        .sheet(isPresented: $artworkManager.showSaveDialog) {
            SaveDialogView(artworkManager: artworkManager)
        }
        .sheet(isPresented: $artworkManager.showLoadDialog) {
            LoadDialogView(artworkManager: artworkManager)
        }
        .sheet(isPresented: $exportManager.showExportDialog) {
            ProfessionalExportPanelView(exportManager: exportManager, errorHandler: errorHandler)
        }
        .alert("Delete Artwork", isPresented: $artworkManager.showDeleteConfirmation) {
            Button("Cancel", role: .cancel) { }
            Button("Delete", role: .destructive) {
                if let artwork = artworkManager.artworkToDelete {
                    artworkManager.deleteArtwork(artwork)
                }
            }
        } message: {
            Text("Are you sure you want to delete this artwork? This action cannot be undone.")
        }
    }
}

struct ProfessionalHeaderView: View {
    @Binding var currentMode: ProfessionalContentView.ProfessionalMode
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Mode selector
            HStack(spacing: 20) {
                ForEach(ProfessionalContentView.ProfessionalMode.allCases, id: \.self) { mode in
                    Button(action: {
                        withAnimation(.spring(response: 0.4, dampingFraction: 0.6)) {
                            currentMode = mode
                        }
                        SafeHapticFeedback.shared.mediumImpact()
                    }) {
                        VStack {
                            mode.icon
                                .font(.system(size: 25))
                                .foregroundColor(currentMode == mode ? .white : .primary)
                                .frame(width: 50, height: 50)
                                .background(
                                    Circle()
                                        .fill(currentMode == mode ? 
                                              LinearGradient(colors: [.blue, .purple], startPoint: .topLeading, endPoint: .bottomTrailing) :
                                              LinearGradient(colors: [Color.gray.opacity(0.2)], startPoint: .topLeading, endPoint: .bottomTrailing)
                                        )
                                )
                                .scaleEffect(currentMode == mode ? 1.1 : 1.0)
                                .shadow(color: currentMode == mode ? .blue.opacity(0.3) : .clear, radius: 5)
                            
                            Text(mode.rawValue)
                                .font(.caption)
                                .fontWeight(currentMode == mode ? .bold : .regular)
                                .foregroundColor(.primary)
                        }
                    }
                    .buttonStyle(PlainButtonStyle())
                }
            }
            
            // Professional status bar
            HStack {
                Text("Professional Mode: \(currentMode.rawValue)")
                    .font(.subheadline)
                    .foregroundColor(.blue)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text("Layers: \(layerManager.layers.count)")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Text("•")
                    .foregroundColor(.secondary)
                
                Text("Zoom: \(Int(canvasManager.canvas.zoomLevel * 100))%")
                    .font(.caption)
                    .foregroundColor(.secondary)
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

struct ProfessionalCanvasView: View {
    let currentMode: ProfessionalContentView.ProfessionalMode
    @Binding var selectedColor: Color
    @ObservedObject var errorHandler: ErrorHandler
    @ObservedObject var undoRedoManager: UndoRedoManager
    @ObservedObject var stickerManager: StickerManager
    @ObservedObject var drawingManager: DrawingManager
    @ObservedObject var layerManager: LayerManager
    @ObservedObject var selectionManager: SelectionManager
    @ObservedObject var transformationManager: TransformationManager
    @ObservedObject var textToolManager: TextToolManager
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @ObservedObject var canvasManager: CanvasManager
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Professional canvas background
                RoundedRectangle(cornerRadius: 20)
                    .fill(canvasManager.canvas.backgroundColor)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Professional canvas area
                    ZStack {
                        // Grid overlay
                        if canvasManager.canvas.gridVisible {
                            GridView(
                                size: canvasManager.canvas.size,
                                gridSize: canvasManager.canvas.gridSize
                            )
                        }
                        
                        // Canvas content based on mode
                        switch currentMode {
                        case .drawing:
                            ProfessionalDrawingCanvasView(
                                drawingManager: drawingManager,
                                layerManager: layerManager,
                                canvasManager: canvasManager
                            )
                        case .coloring:
                            ProfessionalColoringCanvasView(
                                selectedColor: selectedColor,
                                undoRedoManager: undoRedoManager,
                                stickerManager: stickerManager,
                                canvasManager: canvasManager
                            )
                        case .text:
                            ProfessionalTextCanvasView(
                                textToolManager: textToolManager,
                                canvasManager: canvasManager
                            )
                        case .design:
                            ProfessionalDesignCanvasView(
                                gradientPatternManager: gradientPatternManager,
                                canvasManager: canvasManager
                            )
                        }
                        
                        // Selection overlay
                        if !selectionManager.selections.isEmpty {
                            ForEach(selectionManager.selections) { selection in
                                SelectionView(selection: selection, selectionManager: selectionManager)
                            }
                        }
                        
                        // Transformation overlay
                        if transformationManager.isTransforming {
                            TransformationPreviewView(
                                transformationManager: transformationManager,
                                layer: transformationManager.selectedLayer ?? DrawingLayer(name: "Default")
                            )
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    .scaleEffect(canvasManager.canvas.zoomLevel)
                    .offset(canvasManager.canvas.panOffset)
                    .rotationEffect(.degrees(canvasManager.canvas.rotation))
                    .gesture(
                        MagnificationGesture()
                            .onChanged { value in
                                canvasManager.canvas.zoom(to: value)
                            }
                    )
                    .gesture(
                        DragGesture()
                            .onChanged { value in
                                canvasManager.canvas.pan(by: value.translation)
                            }
                    )
                    
                    // Professional status bar
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
                        
                        Text("•")
                            .foregroundColor(.secondary)
                        
                        Text("Mode: \(currentMode.rawValue)")
                            .font(.caption)
                            .foregroundColor(.blue)
                    }
                    .padding(.horizontal)
                    .padding(.bottom, 10)
                }
            }
        }
    }
}

struct ProfessionalDrawingCanvasView: View {
    @ObservedObject var drawingManager: DrawingManager
    @ObservedObject var layerManager: LayerManager
    @ObservedObject var canvasManager: CanvasManager
    
    var body: some View {
        ZStack {
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
    }
}

struct ProfessionalColoringCanvasView: View {
    @Binding var selectedColor: Color
    @ObservedObject var undoRedoManager: UndoRedoManager
    @ObservedObject var stickerManager: StickerManager
    @ObservedObject var canvasManager: CanvasManager
    
    var body: some View {
        ZStack {
            // Colored regions
            ForEach(ExtendedColoringTemplate.butterfly.regions, id: \.id) { region in
                region.path
                    .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                    .onTapGesture {
                        withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                            undoRedoManager.colorRegion(region.id, with: selectedColor)
                        }
                        EnhancedHapticFeedback.shared.colorFill()
                    }
            }
            
            // Template outline
            ExtendedColoringTemplate.butterfly.drawing
                .stroke(Color.black, lineWidth: 3)
            
            // Stickers
            ForEach(stickerManager.stickers) { sticker in
                StickerView(sticker: sticker, stickerManager: stickerManager)
            }
        }
        .onTapGesture { location in
            stickerManager.addSticker(at: location)
            EnhancedHapticFeedback.shared.stickerAdd()
        }
    }
}

struct ProfessionalTextCanvasView: View {
    @ObservedObject var textToolManager: TextToolManager
    @ObservedObject var canvasManager: CanvasManager
    
    var body: some View {
        ZStack {
            // Text elements
            ForEach(textToolManager.textElements) { textElement in
                TextElementView(textElement: textElement, textToolManager: textToolManager)
            }
        }
        .onTapGesture { location in
            textToolManager.addTextElement(at: location)
        }
    }
}

struct ProfessionalDesignCanvasView: View {
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @ObservedObject var canvasManager: CanvasManager
    
    var body: some View {
        ZStack {
            // Gradients
            ForEach(gradientPatternManager.gradients) { gradient in
                GradientElementView(gradientElement: gradient, gradientPatternManager: gradientPatternManager)
            }
            
            // Patterns
            ForEach(gradientPatternManager.patterns) { pattern in
                PatternElementView(patternElement: pattern, gradientPatternManager: gradientPatternManager)
            }
        }
    }
}

struct ProfessionalToolPanelsView: View {
    let currentMode: ProfessionalContentView.ProfessionalMode
    @Binding var selectedColor: Color
    @ObservedObject var errorHandler: ErrorHandler
    @ObservedObject var drawingManager: DrawingManager
    @ObservedObject var layerManager: LayerManager
    @ObservedObject var selectionManager: SelectionManager
    @ObservedObject var transformationManager: TransformationManager
    @ObservedObject var filterManager: FilterManager
    @ObservedObject var textToolManager: TextToolManager
    @ObservedObject var gradientPatternManager: GradientPatternManager
    @ObservedObject var advancedColorManager: AdvancedColorManager
    @ObservedObject var canvasManager: CanvasManager
    @ObservedObject var exportManager: ExportManager
    
    var body: some View {
        ScrollView {
            VStack(spacing: 15) {
                // Mode-specific tool panels
                switch currentMode {
                case .drawing:
                    VStack(spacing: 15) {
                        DrawingToolPalette(drawingManager: drawingManager, errorHandler: errorHandler)
                        LayerPanelView(layerManager: layerManager, errorHandler: errorHandler)
                        SelectionToolPalette(selectionManager: selectionManager, errorHandler: errorHandler)
                        TransformationToolPalette(transformationManager: transformationManager, errorHandler: errorHandler)
                        FilterPanelView(filterManager: filterManager, errorHandler: errorHandler)
                        AdvancedColorPanelView(colorManager: advancedColorManager, errorHandler: errorHandler)
                        CanvasPanelView(canvasManager: canvasManager, errorHandler: errorHandler)
                    }
                case .coloring:
                    VStack(spacing: 15) {
                        EnhancedTemplateSelectorView(selectedTemplate: .constant(.butterfly), errorHandler: errorHandler)
                        ColorMixingPaletteView(colorMixingSystem: ColorMixingSystem(), errorHandler: errorHandler, selectedColor: $selectedColor)
                        StickerPaletteView(stickerManager: StickerManager(), errorHandler: errorHandler)
                        AudioControlsView(audioManager: AudioManager())
                    }
                case .text:
                    VStack(spacing: 15) {
                        TextToolPanel(textToolManager: textToolManager, errorHandler: errorHandler)
                        AdvancedColorPanelView(colorManager: advancedColorManager, errorHandler: errorHandler)
                        CanvasPanelView(canvasManager: canvasManager, errorHandler: errorHandler)
                    }
                case .design:
                    VStack(spacing: 15) {
                        GradientPanelView(gradientPatternManager: gradientPatternManager, errorHandler: errorHandler)
                        PatternPanelView(gradientPatternManager: gradientPatternManager, errorHandler: errorHandler)
                        AdvancedColorPanelView(colorManager: advancedColorManager, errorHandler: errorHandler)
                        CanvasPanelView(canvasManager: canvasManager, errorHandler: errorHandler)
                    }
                }
                
                // Professional export panel
                ProfessionalExportPanelView(exportManager: exportManager, errorHandler: errorHandler)
            }
            .padding(.vertical, 10)
        }
    }
}

#Preview {
    ProfessionalContentView()
        .environmentObject(DashboardManager())
        .environmentObject(ProgressTracker())
        .environmentObject(ArtworkManager())
        .environmentObject(AudioManager())
        .environmentObject(ColorMixingSystem())
        .environmentObject(StickerManager())
        .environmentObject(DrawingManager())
        .environmentObject(LayerManager())
        .environmentObject(SelectionManager())
        .environmentObject(TransformationManager())
        .environmentObject(FilterManager())
        .environmentObject(TextToolManager())
        .environmentObject(GradientPatternManager())
        .environmentObject(AdvancedColorManager())
        .environmentObject(CanvasManager())
        .environmentObject(ExportManager())
}