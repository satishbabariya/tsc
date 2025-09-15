import SwiftUI

@main
struct EnhancedColorFillingApp: App {
    @StateObject private var dashboardManager = DashboardManager()
    @StateObject private var progressTracker = ProgressTracker()
    @StateObject private var artworkManager = ArtworkManager()
    @StateObject private var audioManager = AudioManager()
    @StateObject private var colorMixingSystem = ColorMixingSystem()
    @StateObject private var stickerManager = StickerManager()
    
    var body: some Scene {
        WindowGroup {
            EnhancedContentView()
                .environmentObject(dashboardManager)
                .environmentObject(progressTracker)
                .environmentObject(artworkManager)
                .environmentObject(audioManager)
                .environmentObject(colorMixingSystem)
                .environmentObject(stickerManager)
                .onAppear {
                    setupApp()
                }
        }
    }
    
    private func setupApp() {
        // Initialize audio system
        EnhancedHapticFeedback.shared.setAudioManager(audioManager)
        
        // Start background music if enabled
        if audioManager.isMusicEnabled {
            audioManager.playMusic(audioManager.currentMusicTrack)
        }
        
        // Record app launch
        progressTracker.recordAction(.sessionStart)
    }
}

struct EnhancedContentView: View {
    @EnvironmentObject var dashboardManager: DashboardManager
    @EnvironmentObject var progressTracker: ProgressTracker
    @EnvironmentObject var artworkManager: ArtworkManager
    @EnvironmentObject var audioManager: AudioManager
    @EnvironmentObject var colorMixingSystem: ColorMixingSystem
    @EnvironmentObject var stickerManager: StickerManager
    
    @State private var selectedTemplate: ExtendedColoringTemplate = .butterfly
    @State private var selectedColor: Color = .red
    @State private var animateColors = false
    @StateObject private var errorHandler = ErrorHandler()
    @StateObject private var memoryMonitor = MemoryMonitor()
    @StateObject private var undoRedoManager = UndoRedoManager()
    
    var body: some View {
        NavigationView {
            ZStack {
                VStack(spacing: 0) {
                    // Header with template selection
                    EnhancedTemplateSelectorView(
                        selectedTemplate: $selectedTemplate,
                        errorHandler: errorHandler
                    )
                    
                    // Main coloring area with all features
                    EnhancedColoringCanvasView(
                        template: selectedTemplate,
                        selectedColor: $selectedColor,
                        errorHandler: errorHandler,
                        undoRedoManager: undoRedoManager,
                        stickerManager: stickerManager
                    )
                    
                    // Color mixing palette
                    ColorMixingPaletteView(
                        colorMixingSystem: colorMixingSystem,
                        errorHandler: errorHandler,
                        selectedColor: $selectedColor
                    )
                    
                    // Sticker decorations
                    StickerPaletteView(
                        stickerManager: stickerManager,
                        errorHandler: errorHandler
                    )
                    
                    // Audio controls
                    AudioControlsView(audioManager: audioManager)
                    
                    // Save/Load buttons
                    SaveLoadButtonsView(
                        artworkManager: artworkManager,
                        undoRedoManager: undoRedoManager,
                        stickerManager: stickerManager,
                        selectedTemplate: Binding(
                            get: { ColoringTemplate(rawValue: selectedTemplate.rawValue) ?? .butterfly },
                            set: { _ in }
                        )
                    )
                }
                .navigationTitle("🎨 Color Fun Pro")
                .navigationBarTitleDisplayMode(.inline)
                .toolbar {
                    ToolbarItem(placement: .navigationBarTrailing) {
                        Button(action: {
                            dashboardManager.showDashboard = true
                        }) {
                            Image(systemName: "person.circle")
                                .foregroundColor(.blue)
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
                    progressTracker.recordAction(.sessionEnd(0)) // Duration would be calculated
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

struct EnhancedTemplateSelectorView: View {
    @Binding var selectedTemplate: ExtendedColoringTemplate
    @ObservedObject var errorHandler: ErrorHandler
    @State private var selectedCategory: TemplateCategory = .animals
    
    var body: some View {
        VStack(spacing: 15) {
            // Category selector
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 15) {
                    ForEach(TemplateCategory.allCases, id: \.self) { category in
                        Button(action: {
                            selectedCategory = category
                            SafeHapticFeedback.shared.lightImpact()
                        }) {
                            VStack {
                                category.icon
                                    .font(.system(size: 20))
                                    .foregroundColor(selectedCategory == category ? .white : .primary)
                                
                                Text(category.rawValue)
                                    .font(.caption)
                                    .foregroundColor(selectedCategory == category ? .white : .primary)
                            }
                            .padding(.horizontal, 12)
                            .padding(.vertical, 8)
                            .background(
                                RoundedRectangle(cornerRadius: 10)
                                    .fill(selectedCategory == category ? Color.blue : Color.gray.opacity(0.2))
                            )
                        }
                        .buttonStyle(PlainButtonStyle())
                    }
                }
                .padding(.horizontal)
            }
            
            // Template selector
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 15) {
                    ForEach(ExtendedColoringTemplate.allCases.filter { $0.category == selectedCategory }, id: \.self) { template in
                        Button(action: {
                            // Validate template before selection
                            guard InputValidator.validateTemplate(ColoringTemplate(rawValue: template.rawValue) ?? .butterfly) else {
                                errorHandler.handleError(.invalidTemplate)
                                return
                            }
                            
                            withAnimation(.spring(response: 0.4, dampingFraction: 0.6)) {
                                selectedTemplate = template
                            }
                            
                            // Record template usage
                            progressTracker.recordAction(.useTemplate(template.name))
                            
                            // Safe haptic feedback
                            SafeHapticFeedback.shared.mediumImpact()
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

struct EnhancedColoringCanvasView: View {
    let template: ExtendedColoringTemplate
    @Binding var selectedColor: Color
    @ObservedObject var errorHandler: ErrorHandler
    @ObservedObject var undoRedoManager: UndoRedoManager
    @ObservedObject var stickerManager: StickerManager
    @State private var showShareSheet = false
    @State private var capturedImage: UIImage?
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Template drawing with colored regions and stickers
                    ZStack {
                        // Colored regions (behind the outline)
                        ForEach(template.regions, id: \.id) { region in
                            region.path
                                .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                                .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                                .onTapGesture {
                                    // Validate region before coloring
                                    guard InputValidator.validateRegionId(region.id) else {
                                        errorHandler.handleError(.invalidRegion)
                                        return
                                    }
                                    
                                    withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                                        undoRedoManager.colorRegion(region.id, with: selectedColor)
                                    }
                                    
                                    // Record action
                                    progressTracker.recordAction(.colorRegion)
                                    progressTracker.recordAction(.useColor(selectedColor))
                                    
                                    // Safe haptic feedback with audio
                                    EnhancedHapticFeedback.shared.colorFill()
                                }
                        }
                        
                        // Template outline (on top)
                        template.drawing
                            .stroke(Color.black, lineWidth: 3)
                            .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                        
                        // Stickers
                        ForEach(stickerManager.stickers) { sticker in
                            StickerView(sticker: sticker, stickerManager: stickerManager)
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    .onTapGesture { location in
                        // Add sticker at tap location
                        let adjustedLocation = CGPoint(
                            x: location.x,
                            y: location.y
                        )
                        stickerManager.addSticker(at: adjustedLocation)
                        
                        // Record action
                        progressTracker.recordAction(.addSticker)
                        
                        // Audio feedback
                        EnhancedHapticFeedback.shared.stickerAdd()
                    }
                    
                    // Undo/Redo buttons
                    UndoRedoButtonsView(undoRedoManager: undoRedoManager, errorHandler: errorHandler)
                    
                    // Action buttons
                    HStack(spacing: 20) {
                        // Clear button
                        Button(action: {
                            withAnimation(.spring()) {
                                undoRedoManager.clearAll()
                                stickerManager.clearAllStickers()
                            }
                            EnhancedHapticFeedback.shared.clear()
                        }) {
                            HStack {
                                Image(systemName: "trash")
                                Text("Clear")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.red)
                            .cornerRadius(20)
                        }
                        
                        // Save/Share button
                        Button(action: {
                            guard let image = captureImage() else {
                                errorHandler.handleError(.imageCaptureFailed)
                                return
                            }
                            capturedImage = image
                            showShareSheet = true
                            
                            // Record artwork creation
                            progressTracker.recordAction(.createArtwork)
                        }) {
                            HStack {
                                Image(systemName: "square.and.arrow.up")
                                Text("Share")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.blue)
                            .cornerRadius(20)
                        }
                    }
                    .padding(.bottom, 10)
                }
            }
        }
        .onChange(of: template) { _ in
            withAnimation(.easeInOut) {
                undoRedoManager.reset()
                stickerManager.clearAllStickers()
            }
        }
        .sheet(isPresented: $showShareSheet) {
            if let image = capturedImage {
                SafeShareSheet(activityItems: [image], errorHandler: errorHandler)
            }
        }
    }
    
    private func captureImage() -> UIImage? {
        let content = ZStack {
            Color.white
            VStack {
                ZStack {
                    // Colored regions
                    ForEach(template.regions, id: \.id) { region in
                        region.path
                            .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                    }
                    
                    // Template outline
                    template.drawing
                        .stroke(Color.black, lineWidth: 3)
                    
                    // Stickers
                    ForEach(stickerManager.stickers) { sticker in
                        sticker.type.icon
                            .font(.system(size: sticker.size))
                            .foregroundColor(sticker.color)
                            .rotationEffect(.degrees(sticker.rotation))
                            .position(sticker.position)
                    }
                }
                .frame(width: 400, height: 400)
            }
        }
        
        return SafeImageCapture.captureImage(from: content, errorHandler: errorHandler)
    }
}

#Preview {
    EnhancedContentView()
        .environmentObject(DashboardManager())
        .environmentObject(ProgressTracker())
        .environmentObject(ArtworkManager())
        .environmentObject(AudioManager())
        .environmentObject(ColorMixingSystem())
        .environmentObject(StickerManager())
}