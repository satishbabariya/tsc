import SwiftUI

// MARK: - Artwork Model

struct SavedArtwork: Identifiable, Codable {
    let id = UUID()
    let name: String
    let template: String
    let coloredRegions: [String: String] // Color as string for Codable
    let stickers: [SavedSticker]
    let createdAt: Date
    let modifiedAt: Date
    let thumbnailData: Data?
    
    init(name: String, template: String, coloredRegions: [String: Color], stickers: [Sticker], thumbnailData: Data? = nil) {
        self.name = name
        self.template = template
        self.coloredRegions = coloredRegions.mapValues { $0.description }
        self.stickers = stickers.map { SavedSticker(from: $0) }
        self.createdAt = Date()
        self.modifiedAt = Date()
        self.thumbnailData = thumbnailData
    }
    
    func getColoredRegionsAsColors() -> [String: Color] {
        return coloredRegions.compactMapValues { colorString in
            // Convert string back to Color (simplified)
            switch colorString {
            case "red": return .red
            case "blue": return .blue
            case "green": return .green
            case "yellow": return .yellow
            case "orange": return .orange
            case "purple": return .purple
            case "pink": return .pink
            case "brown": return .brown
            case "gray": return .gray
            case "black": return .black
            case "white": return .white
            case "cyan": return .cyan
            case "mint": return .mint
            case "teal": return .teal
            case "indigo": return .indigo
            default: return .red // fallback
            }
        }
    }
}

struct SavedSticker: Codable {
    let type: String
    let positionX: Double
    let positionY: Double
    let size: Double
    let rotation: Double
    let color: String
    
    init(from sticker: Sticker) {
        self.type = sticker.type.rawValue
        self.positionX = Double(sticker.position.x)
        self.positionY = Double(sticker.position.y)
        self.size = Double(sticker.size)
        self.rotation = Double(sticker.rotation)
        self.color = sticker.color.description
    }
    
    func toSticker() -> Sticker {
        return Sticker(
            type: StickerType(rawValue: type) ?? .star,
            position: CGPoint(x: positionX, y: positionY),
            size: CGFloat(size),
            rotation: Double(rotation),
            color: colorStringToColor(color)
        )
    }
    
    private func colorStringToColor(_ colorString: String) -> Color {
        switch colorString {
        case "red": return .red
        case "blue": return .blue
        case "green": return .green
        case "yellow": return .yellow
        case "orange": return .orange
        case "purple": return .purple
        case "pink": return .pink
        case "brown": return .brown
        case "gray": return .gray
        case "black": return .black
        case "white": return .white
        case "cyan": return .cyan
        case "mint": return .mint
        case "teal": return .teal
        case "indigo": return .indigo
        default: return .red
        }
    }
}

// MARK: - Artwork Manager

class ArtworkManager: ObservableObject {
    @Published var savedArtworks: [SavedArtwork] = []
    @Published var currentArtwork: SavedArtwork?
    @Published var showSaveDialog = false
    @Published var showLoadDialog = false
    @Published var showDeleteConfirmation = false
    @Published var artworkToDelete: SavedArtwork?
    
    private let maxSaveSlots = 50
    
    init() {
        loadSavedArtworks()
    }
    
    func saveArtwork(name: String, template: String, coloredRegions: [String: Color], stickers: [Sticker]) {
        // Create thumbnail
        let thumbnailData = createThumbnail(template: template, coloredRegions: coloredRegions, stickers: stickers)
        
        let artwork = SavedArtwork(
            name: name,
            template: template,
            coloredRegions: coloredRegions,
            stickers: stickers,
            thumbnailData: thumbnailData
        )
        
        // Check if we're updating an existing artwork
        if let current = currentArtwork {
            if let index = savedArtworks.firstIndex(where: { $0.id == current.id }) {
                savedArtworks[index] = artwork
            }
        } else {
            // Add new artwork
            if savedArtworks.count >= maxSaveSlots {
                // Remove oldest artwork
                savedArtworks.removeFirst()
            }
            savedArtworks.append(artwork)
        }
        
        currentArtwork = artwork
        saveToStorage()
    }
    
    func loadArtwork(_ artwork: SavedArtwork) {
        currentArtwork = artwork
    }
    
    func deleteArtwork(_ artwork: SavedArtwork) {
        savedArtworks.removeAll { $0.id == artwork.id }
        if currentArtwork?.id == artwork.id {
            currentArtwork = nil
        }
        saveToStorage()
    }
    
    func duplicateArtwork(_ artwork: SavedArtwork) {
        let duplicatedArtwork = SavedArtwork(
            name: "\(artwork.name) Copy",
            template: artwork.template,
            coloredRegions: artwork.coloredRegions,
            stickers: artwork.stickers,
            thumbnailData: artwork.thumbnailData
        )
        
        if savedArtworks.count < maxSaveSlots {
            savedArtworks.append(duplicatedArtwork)
            saveToStorage()
        }
    }
    
    func renameArtwork(_ artwork: SavedArtwork, newName: String) {
        if let index = savedArtworks.firstIndex(where: { $0.id == artwork.id }) {
            var updatedArtwork = savedArtworks[index]
            // Note: This would require making SavedArtwork properties mutable
            // For now, we'll recreate the artwork with the new name
            let renamedArtwork = SavedArtwork(
                name: newName,
                template: updatedArtwork.template,
                coloredRegions: updatedArtwork.coloredRegions,
                stickers: updatedArtwork.stickers,
                thumbnailData: updatedArtwork.thumbnailData
            )
            savedArtworks[index] = renamedArtwork
            saveToStorage()
        }
    }
    
    private func createThumbnail(template: String, coloredRegions: [String: Color], stickers: [Sticker]) -> Data? {
        // Create a small thumbnail image
        let size = CGSize(width: 100, height: 100)
        let renderer = UIGraphicsImageRenderer(size: size)
        
        let image = renderer.image { context in
            // Draw template and colors
            context.cgContext.setFillColor(UIColor.white.cgColor)
            context.cgContext.fill(CGRect(origin: .zero, size: size))
            
            // Draw colored regions (simplified)
            for (_, color) in coloredRegions {
                context.cgContext.setFillColor(UIColor(color).cgColor)
                context.cgContext.fillEllipse(in: CGRect(x: 20, y: 20, width: 60, height: 60))
            }
        }
        
        return image.pngData()
    }
    
    private func saveToStorage() {
        if let encoded = try? JSONEncoder().encode(savedArtworks) {
            UserDefaults.standard.set(encoded, forKey: "savedArtworks")
        }
    }
    
    private func loadSavedArtworks() {
        if let data = UserDefaults.standard.data(forKey: "savedArtworks"),
           let decoded = try? JSONDecoder().decode([SavedArtwork].self, from: data) {
            savedArtworks = decoded
        }
    }
}

// MARK: - Save Dialog View

struct SaveDialogView: View {
    @ObservedObject var artworkManager: ArtworkManager
    @State private var artworkName: String = ""
    @State private var showNameError = false
    
    var body: some View {
        VStack(spacing: 20) {
            Text("💾 Save Artwork")
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            VStack(alignment: .leading, spacing: 10) {
                Text("Artwork Name")
                    .font(.headline)
                    .foregroundColor(.primary)
                
                TextField("Enter artwork name", text: $artworkName)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                    .onChange(of: artworkName) { _ in
                        showNameError = false
                    }
                
                if showNameError {
                    Text("Please enter a name for your artwork")
                        .font(.caption)
                        .foregroundColor(.red)
                }
            }
            
            HStack(spacing: 15) {
                Button("Cancel") {
                    artworkManager.showSaveDialog = false
                }
                .buttonStyle(.bordered)
                
                Button("Save") {
                    if artworkName.isEmpty {
                        showNameError = true
                    } else {
                        // This would be called with actual artwork data
                        artworkManager.showSaveDialog = false
                    }
                }
                .buttonStyle(.borderedProminent)
            }
        }
        .padding()
        .background(Color.white)
        .cornerRadius(20)
        .shadow(radius: 10)
    }
}

// MARK: - Load Dialog View

struct LoadDialogView: View {
    @ObservedObject var artworkManager: ArtworkManager
    @State private var searchText = ""
    
    var body: some View {
        VStack(spacing: 20) {
            Text("📁 Load Artwork")
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            // Search bar
            HStack {
                Image(systemName: "magnifyingglass")
                    .foregroundColor(.secondary)
                
                TextField("Search artworks", text: $searchText)
                    .textFieldStyle(PlainTextFieldStyle())
            }
            .padding()
            .background(Color.gray.opacity(0.1))
            .cornerRadius(10)
            
            // Artworks list
            ScrollView {
                LazyVStack(spacing: 10) {
                    ForEach(filteredArtworks) { artwork in
                        ArtworkRowView(artwork: artwork, artworkManager: artworkManager)
                    }
                }
            }
            .frame(maxHeight: 400)
            
            Button("Cancel") {
                artworkManager.showLoadDialog = false
            }
            .buttonStyle(.bordered)
        }
        .padding()
        .background(Color.white)
        .cornerRadius(20)
        .shadow(radius: 10)
    }
    
    private var filteredArtworks: [SavedArtwork] {
        if searchText.isEmpty {
            return artworkManager.savedArtworks
        } else {
            return artworkManager.savedArtworks.filter { artwork in
                artwork.name.localizedCaseInsensitiveContains(searchText)
            }
        }
    }
}

// MARK: - Artwork Row View

struct ArtworkRowView: View {
    let artwork: SavedArtwork
    @ObservedObject var artworkManager: ArtworkManager
    @State private var showRenameDialog = false
    @State private var newName = ""
    
    var body: some View {
        HStack(spacing: 15) {
            // Thumbnail
            if let thumbnailData = artwork.thumbnailData,
               let uiImage = UIImage(data: thumbnailData) {
                Image(uiImage: uiImage)
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 60, height: 60)
                    .cornerRadius(8)
            } else {
                RoundedRectangle(cornerRadius: 8)
                    .fill(Color.gray.opacity(0.3))
                    .frame(width: 60, height: 60)
                    .overlay(
                        Image(systemName: "photo")
                            .foregroundColor(.gray)
                    )
            }
            
            // Artwork info
            VStack(alignment: .leading, spacing: 5) {
                Text(artwork.name)
                    .font(.headline)
                    .foregroundColor(.primary)
                
                Text(artwork.template)
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Text("Created: \(artwork.createdAt, style: .date)")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            // Action buttons
            HStack(spacing: 10) {
                Button(action: {
                    artworkManager.loadArtwork(artwork)
                    artworkManager.showLoadDialog = false
                }) {
                    Image(systemName: "arrow.down.circle.fill")
                        .foregroundColor(.blue)
                        .font(.system(size: 25))
                }
                
                Button(action: {
                    newName = artwork.name
                    showRenameDialog = true
                }) {
                    Image(systemName: "pencil.circle.fill")
                        .foregroundColor(.orange)
                        .font(.system(size: 25))
                }
                
                Button(action: {
                    artworkManager.duplicateArtwork(artwork)
                }) {
                    Image(systemName: "doc.on.doc.fill")
                        .foregroundColor(.green)
                        .font(.system(size: 25))
                }
                
                Button(action: {
                    artworkManager.artworkToDelete = artwork
                    artworkManager.showDeleteConfirmation = true
                }) {
                    Image(systemName: "trash.circle.fill")
                        .foregroundColor(.red)
                        .font(.system(size: 25))
                }
            }
        }
        .padding()
        .background(Color.gray.opacity(0.05))
        .cornerRadius(10)
        .alert("Rename Artwork", isPresented: $showRenameDialog) {
            TextField("New name", text: $newName)
            Button("Cancel", role: .cancel) { }
            Button("Rename") {
                artworkManager.renameArtwork(artwork, newName: newName)
            }
        }
    }
}

// MARK: - Save/Load Buttons View

struct SaveLoadButtonsView: View {
    @ObservedObject var artworkManager: ArtworkManager
    @ObservedObject var undoRedoManager: UndoRedoManager
    @ObservedObject var stickerManager: StickerManager
    @Binding var selectedTemplate: ColoringTemplate
    
    var body: some View {
        HStack(spacing: 15) {
            // Save button
            Button(action: {
                artworkManager.showSaveDialog = true
            }) {
                HStack {
                    Image(systemName: "square.and.arrow.down")
                    Text("Save")
                }
                .foregroundColor(.white)
                .padding(.horizontal, 15)
                .padding(.vertical, 8)
                .background(Color.green)
                .cornerRadius(15)
            }
            
            // Load button
            Button(action: {
                artworkManager.showLoadDialog = true
            }) {
                HStack {
                    Image(systemName: "square.and.arrow.up")
                    Text("Load")
                }
                .foregroundColor(.white)
                .padding(.horizontal, 15)
                .padding(.vertical, 8)
                .background(Color.blue)
                .cornerRadius(15)
            }
            
            Spacer()
            
            // Current artwork info
            if let currentArtwork = artworkManager.currentArtwork {
                Text("📁 \(currentArtwork.name)")
                    .font(.caption)
                    .foregroundColor(.secondary)
            } else {
                Text("📁 New Artwork")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding(.horizontal)
    }
}

// MARK: - Artwork Gallery View

struct ArtworkGalleryView: View {
    @ObservedObject var artworkManager: ArtworkManager
    @State private var selectedArtwork: SavedArtwork?
    
    var body: some View {
        VStack(spacing: 20) {
            Text("🖼️ Artwork Gallery")
                .font(.title)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            if artworkManager.savedArtworks.isEmpty {
                VStack(spacing: 15) {
                    Image(systemName: "photo.on.rectangle")
                        .font(.system(size: 50))
                        .foregroundColor(.gray)
                    
                    Text("No saved artworks yet")
                        .font(.headline)
                        .foregroundColor(.secondary)
                    
                    Text("Create your first masterpiece and save it!")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                .padding()
            } else {
                ScrollView {
                    LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 3), spacing: 15) {
                        ForEach(artworkManager.savedArtworks) { artwork in
                            ArtworkThumbnailView(artwork: artwork, artworkManager: artworkManager)
                        }
                    }
                    .padding(.horizontal)
                }
            }
        }
        .padding(.vertical)
    }
}

// MARK: - Artwork Thumbnail View

struct ArtworkThumbnailView: View {
    let artwork: SavedArtwork
    @ObservedObject var artworkManager: ArtworkManager
    @State private var showDetailView = false
    
    var body: some View {
        VStack(spacing: 10) {
            // Thumbnail
            if let thumbnailData = artwork.thumbnailData,
               let uiImage = UIImage(data: thumbnailData) {
                Image(uiImage: uiImage)
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 100, height: 100)
                    .cornerRadius(10)
                    .onTapGesture {
                        showDetailView = true
                    }
            } else {
                RoundedRectangle(cornerRadius: 10)
                    .fill(Color.gray.opacity(0.3))
                    .frame(width: 100, height: 100)
                    .overlay(
                        Image(systemName: "photo")
                            .foregroundColor(.gray)
                    )
            }
            
            // Artwork name
            Text(artwork.name)
                .font(.caption)
                .foregroundColor(.primary)
                .lineLimit(2)
                .multilineTextAlignment(.center)
            
            // Template name
            Text(artwork.template)
                .font(.caption2)
                .foregroundColor(.secondary)
        }
        .sheet(isPresented: $showDetailView) {
            ArtworkDetailView(artwork: artwork, artworkManager: artworkManager)
        }
    }
}

// MARK: - Artwork Detail View

struct ArtworkDetailView: View {
    let artwork: SavedArtwork
    @ObservedObject var artworkManager: ArtworkManager
    @Environment(\.dismiss) private var dismiss
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                // Large thumbnail
                if let thumbnailData = artwork.thumbnailData,
                   let uiImage = UIImage(data: thumbnailData) {
                    Image(uiImage: uiImage)
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(maxWidth: 300, maxHeight: 300)
                        .cornerRadius(15)
                        .shadow(radius: 5)
                }
                
                // Artwork info
                VStack(spacing: 10) {
                    Text(artwork.name)
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(.primary)
                    
                    Text("Template: \(artwork.template)")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                    
                    Text("Created: \(artwork.createdAt, style: .date)")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Modified: \(artwork.modifiedAt, style: .date)")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                // Action buttons
                HStack(spacing: 20) {
                    Button("Load") {
                        artworkManager.loadArtwork(artwork)
                        dismiss()
                    }
                    .buttonStyle(.borderedProminent)
                    
                    Button("Duplicate") {
                        artworkManager.duplicateArtwork(artwork)
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Delete", role: .destructive) {
                        artworkManager.deleteArtwork(artwork)
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                }
                
                Spacer()
            }
            .padding()
            .navigationTitle("Artwork Details")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarTrailing) {
                    Button("Done") {
                        dismiss()
                    }
                }
            }
        }
    }
}